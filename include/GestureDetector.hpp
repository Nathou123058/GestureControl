#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <memory>

enum class GestureType {
    HAND_RAISED,
    HAND_LOWERED,
    NONE
};

struct HandData {
    bool detected;
    float confidence;
    float x, y;
    GestureType gesture;
};

class GestureDetector {
public:
    GestureDetector();
    ~GestureDetector();
    
    // Initialize detector
    bool initialize(float confidenceThreshold = 0.7f);
    
    // Detect hand gestures from frame
    void detectGestures(const cv::Mat& frame, HandData& leftHand, HandData& rightHand);
    
    // Get current detection status
    bool isActive() const { return m_active; }
    void setActive(bool active) { m_active = active; }
    
    // Set confidence threshold
    void setConfidenceThreshold(float threshold) { m_confidenceThreshold = threshold; }

private:
    bool m_active;
    float m_confidenceThreshold;
    
    // Helper functions
    bool detectHandPose(const cv::Mat& frame, HandData& hand, bool isLeftHand);
    GestureType classifyGesture(float handY, float frameHeight);
};
