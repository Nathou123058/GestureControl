#include <geode/Geode.hpp>
#include <geode/modify/PlayLayer.hpp>
#include "WebcamHandler.hpp"

using namespace geode::prelude;

static std::unique_ptr<WebcamHandler> g_webcamHandler;

class $modify(PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
            return false;
        }
        
        // Initialize webcam handler on first play
        if (!g_webcamHandler) {
            auto mod = Mod::get();
            
            g_webcamHandler = std::make_unique<WebcamHandler>();
            
            int cameraIndex = mod->getSettingValue<int64_t>("webcam_index");
            if (!g_webcamHandler->initialize(static_cast<int>(cameraIndex))) {
                log::warn("Failed to initialize webcam");
                g_webcamHandler.reset();
                return true;
            }
            
            float confidence = mod->getSettingValue<double>("confidence_threshold");
            g_webcamHandler->setConfidenceThreshold(static_cast<float>(confidence));
            
            bool enabled = mod->getSettingValue<bool>("enable_gestures");
            g_webcamHandler->setEnabled(enabled);
            
            g_webcamHandler->start();
        }
        
        return true;
    }
    
    void update(float dt) {
        PlayLayer::update(dt);
        
        if (!g_webcamHandler || !g_webcamHandler->isRunning()) {
            return;
        }
        
        // Get gesture data
        HandData leftHand, rightHand;
        g_webcamHandler->getGestureData(leftHand, rightHand);
        
        auto mod = Mod::get();
        std::string leftAction = mod->getSettingValue<std::string>("left_hand_action");
        std::string rightAction = mod->getSettingValue<std::string>("right_hand_action");
        
        // Process left hand gesture
        if (leftHand.detected && leftHand.gesture == GestureType::HAND_RAISED) {
            if (leftAction == "jump") {
                this->handleButtonPress(PlayerButton::Jump);
            }
        }
        
        // Process right hand gesture
        if (rightHand.detected && rightHand.gesture == GestureType::HAND_RAISED) {
            if (rightAction == "jump") {
                this->handleButtonPress(PlayerButton::Jump);
            }
        }
    }
    
    void onQuit() {
        // Stop webcam on quit
        if (g_webcamHandler) {
            g_webcamHandler->stop();
        }
        return PlayLayer::onQuit();
    }
};

// Settings listener
void updateSettings() {
    if (!g_webcamHandler) return;
    
    auto mod = Mod::get();
    
    bool enabled = mod->getSettingValue<bool>("enable_gestures");
    g_webcamHandler->setEnabled(enabled);
    
    float confidence = mod->getSettingValue<double>("confidence_threshold");
    g_webcamHandler->setConfidenceThreshold(confidence);
}

$on_mod(Loaded) {
    auto mod = Mod::get();
    
    // Listen to setting changes
    mod->getSettingValue<bool>("enable_gestures")->setFilter([](bool value) {
        updateSettings();
        return value;
    });
    
    mod->getSettingValue<double>("confidence_threshold")->setFilter([](double value) {
        updateSettings();
        return value;
    });
    
    log::info("GestureControl mod loaded!");
}
