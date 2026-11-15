#include "TestLayer.h"

TestLayer::TestLayer()
    : Layer("Example") {
}

void TestLayer::onAttach() {
    BN_INFO("Layer {0} attached", m_debugName);
}

void TestLayer::onDetach() {
    BN_INFO("Layer {0} detached", m_debugName);
}

void TestLayer::onUpdate() {
}

void TestLayer::onEvent(Event& event) {
}

void TestLayer::onImGuiRender() {
}
