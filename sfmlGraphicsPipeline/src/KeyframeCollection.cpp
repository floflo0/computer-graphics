# include <KeyframeCollection.hpp>

# include <glm/gtx/compatibility.hpp>

void KeyframeCollection::add(const GeometricTransformation& transformation,
                             const float time) {
    m_keyframes.insert(std::make_pair(time, transformation));
}

glm::mat4 KeyframeCollection::interpolateTransformation(const float time) const
{
    if (m_keyframes.empty()) return glm::mat4(1.0);

    // Handle the case where the time parameter is outside the keyframes time
    // scope.
    const std::map<float, GeometricTransformation>::const_iterator
        itFirstFrame = m_keyframes.begin();
    const std::map<float, GeometricTransformation>::const_reverse_iterator
        itLastFrame = m_keyframes.rbegin();
    const float effective_time = std::max(
        std::fmod(time, itLastFrame->first),
        itFirstFrame->first
    );

    // Get keyframes surrounding the time parameter.
    const std::array<Keyframe, 2> result = getBoundingKeyframes(effective_time);

    const float t0 = result[0].first;
    const float t1 = result[1].first;
    const float f = (effective_time - t0) / (t1 - t0);

    const GeometricTransformation *const xt0 = &result[0].second;
    const GeometricTransformation *const xt1 = &result[1].second;

    GeometricTransformation tranformation(
        glm::lerp(xt0->getTranslation(), xt1->getTranslation(), f),
        glm::slerp(xt0->getOrientation(), xt1->getOrientation(), f),
        glm::lerp(xt0->getScale(), xt1->getScale(), f)
    );

    return tranformation.toMatrix();
}

bool KeyframeCollection::empty() const {
    return m_keyframes.empty();
}

const std::map<float, GeometricTransformation>
&KeyframeCollection::getKeyFrames() const {
    return m_keyframes;
}

std::array<KeyframeCollection::Keyframe, 2>
KeyframeCollection::getBoundingKeyframes(const float time) const {
    std::array< KeyframeCollection::Keyframe, 2 > result{
        std::make_pair(0, GeometricTransformation()),
            std::make_pair(0, GeometricTransformation())
    };
    std::map<float, GeometricTransformation>::const_iterator upper =
        m_keyframes.upper_bound(time);
    std::map<float, GeometricTransformation>::const_iterator lower =
        std::prev(upper);
    std::map<float, GeometricTransformation>::const_iterator end =
        m_keyframes.end();
    if (upper != end && lower != end ) {
        result[0] = *lower;
        result[1] = *upper;
    }
    return result;
}
