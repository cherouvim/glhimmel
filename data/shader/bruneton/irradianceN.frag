#version 330
#extension GL_ARB_shading_language_include : require
#include </data/shader/bruneton/include/constants.glsl>
#include </data/shader/bruneton/include/uniforms.glsl>
#include </data/shader/bruneton/include/irradianceRMuS.glsl>
#include </data/shader/bruneton/include/phaseFunction.glsl>
#include </data/shader/bruneton/include/texture4D.glsl>

uniform sampler3D deltaSRSampler;
uniform sampler3D deltaSMSampler;
uniform float first;

const float dphi = PI / float(IRRADIANCE_INTEGRAL_SAMPLES);
const float dtheta = PI / float(IRRADIANCE_INTEGRAL_SAMPLES);

out vec4 out_color;

void main() {
    float r, muS;
    getIrradianceRMuS(r, muS);
    vec3 s = vec3(max(sqrt(1.0 - muS * muS), 0.0), 0.0, muS);

    vec3 result = vec3(0.0);
    // integral over 2.PI around x with two nested loops over w directions (theta,phi) -- Eq (15)
    for (int iphi = 0; iphi < 2 * IRRADIANCE_INTEGRAL_SAMPLES; ++iphi) {
        float phi = (float(iphi) + 0.5) * dphi;
        for (int itheta = 0; itheta < IRRADIANCE_INTEGRAL_SAMPLES / 2; ++itheta) {
            float theta = (float(itheta) + 0.5) * dtheta;
            float dw = dtheta * dphi * sin(theta);
            vec3 w = vec3(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta));
            float nu = dot(s, w);
            if (first == 1.0) {
                // first iteration is special because Rayleigh and Mie were stored separately,
                // without the phase functions factors; they must be reintroduced here
                float pr1 = phaseFunctionR(nu);
                float pm1 = phaseFunctionM(nu);
                vec3 ray1 = texture4D(deltaSRSampler, r, w.z, muS, nu).rgb;
                vec3 mie1 = texture4D(deltaSMSampler, r, w.z, muS, nu).rgb;
                result += (ray1 * pr1 + mie1 * pm1) * w.z * dw;
            } else {
                result += texture4D(deltaSRSampler, r, w.z, muS, nu).rgb * w.z * dw;
            }
        }
    }

    out_color = vec4(result, 0.0);
}