//
// Basic instrumentation profiler by Cherno

// Usage: include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// Instrumentor::Get().BeginSession("Session Name");        // Begin session
// {
//     InstrumentationTimer timer("Profiled Scope Name");   // Place code like this in scopes you'd like to include in profiling
//     // Code
// }
// Instrumentor::Get().EndSession();                        // End Session
//
// You will probably want to macro-fy this, to switch on/off easily and use things like __FUNCSIG__ for the profile name.
//
#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

namespace Bottleneck {
    struct ProfileResult {
        std::string Name;
        long long Start, End;
        uint32_t ThreadID;
    };

    struct InstrumentationSession {
        std::string Name;
    };

    class Instrumentor {
        InstrumentationSession* m_currentSession;
        std::ofstream m_outputStream;
        int m_profileCount;

        Instrumentor()
            : m_currentSession(nullptr), m_profileCount(0) {
        }

    public:
        void BeginSession(const std::string& name, const std::string& filepath = "results.json") {
            m_outputStream.open(filepath);
            WriteHeader();
            m_currentSession = new InstrumentationSession{name};
        }

        void EndSession() {
            WriteFooter();
            m_outputStream.close();
            delete m_currentSession;
            m_currentSession = nullptr;
            m_profileCount = 0;
        }

        void WriteProfile(const ProfileResult& result) {
            if (m_profileCount++ > 0)
                m_outputStream << ",";

            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            m_outputStream << "{";
            m_outputStream << "\"cat\":\"function\",";
            m_outputStream << "\"dur\":" << (result.End - result.Start) << ',';
            m_outputStream << "\"name\":\"" << name << "\",";
            m_outputStream << "\"ph\":\"X\",";
            m_outputStream << "\"pid\":0,";
            m_outputStream << "\"tid\":" << result.ThreadID << ",";
            m_outputStream << "\"ts\":" << result.Start;
            m_outputStream << "}";
        }

        void WriteHeader() {
            m_outputStream << "{\"otherData\": {},\"traceEvents\":[";
        }

        void WriteFooter() {
            m_outputStream << "]}";
        }

        static Instrumentor& Get() {
            static Instrumentor instance;
            return instance;
        }
    };

    class InstrumentationTimer {
        const char* m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
        bool m_Stopped;

    public:
        InstrumentationTimer(const char* name)
            : m_Name(name), m_Stopped(false) {
            m_StartTimepoint = std::chrono::high_resolution_clock::now();
        }

        ~InstrumentationTimer() {
            if (!m_Stopped)
                Stop();
        }

        void Stop() {
            const auto endTimepoint = std::chrono::high_resolution_clock::now();
            const long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).
                    time_since_epoch().count();
            const long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).
                    time_since_epoch().count();

            const uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
            Instrumentor::Get().WriteProfile({m_Name, start, end, threadID});

            m_Stopped = true;
        }
    };
}

#define BN_PROFILE 0
#if BN_PROFILE
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define BN_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define BN_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define BN_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define BN_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define BN_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define BN_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define BN_FUNC_SIG __func__
#else
#define BN_FUNC_SIG "BN_FUNC_SIG unknown!"
#endif

#define BN_PROFILE_BEGIN_SESSION(name, filepath) ::Bottleneck::Instrumentor::Get().BeginSession(name, filepath)
#define BN_PROFILE_END_SESSION() ::Bottleneck::Instrumentor::Get().EndSession()
#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)
#define BN_PROFILE_SCOPE(name) ::Bottleneck::InstrumentationTimer CONCAT(timer, __LINE__(name))

#define BN_PROFILE_FUNCTION() BN_PROFILE_SCOPE(BN_FUNC_SIG)
#else
#define BN_PROFILE_BEGIN_SESSION(name, filepath)
#define BN_PROFILE_END_SESSION()
#define BN_PROFILE_SCOPE(name)
#define BN_PROFILE_FUNCTION()
#endif
