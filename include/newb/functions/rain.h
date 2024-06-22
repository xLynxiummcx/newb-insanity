#ifndef RAIN_H
#define RAIN_H

#include "noise.h"
#include "sky.h"
#include "water.h"

// Constants for wind dynamics (adjust as needed)
#define NL_WIND_FREQUENCY 4.0
#define NL_WIND_AMPLITUDE 2.0
#define NL_TIME_FACTOR 0.2

float nlWindblow(vec2 p, float t){
  float windFrequency = NL_WIND_FREQUENCY;
  float windAmplitude = NL_WIND_AMPLITUDE;
  float timeFactor = NL_TIME_FACTOR;
  
  float val = sin(windFrequency * p.x + 0.5 * windFrequency * p.y + timeFactor * t + windAmplitude * p.y * p.x);
  val += sin(p.y - p.x + timeFactor * t);
  
  return 0.25 * val * val;
}

vec4 nlRefl(
  inout vec4 color, inout vec4 mistColor, vec2 lit, vec2 uv1, vec3 tiledCpos,
  float camDist, vec3 wPos, vec3 viewDir, vec3 torchColor, vec3 horizonEdgeCol, vec3 horizonCol,
  vec3 zenithCol, vec3 FOG_COLOR, float rainFactor, float renderDist, highp float t, vec3 pos, bool underWater, bool end, bool nether
) {
  vec4 wetRefl = vec4(0.0, 0.0, 0.0, 0.0);

  #ifndef NL_GROUND_REFL
  if (rainFactor > 0.0) {
  #endif
    float wetness = lit.y * lit.y;

    #ifdef NL_RAIN_MIST_OPACITY
      // humid air blow
      float humidAir = rainFactor * wetness * nlWindblow(pos.xy / (1.0 + pos.z), t);
      mistColor.a = min(mistColor.a + humidAir * NL_RAIN_MIST_OPACITY, 1.0);
    #endif

    // clip reflection when far (better performance)
    float endDist = renderDist * 0.6;
    if (camDist < endDist) {
      float cosR = max(viewDir.y, 0.0);
      float puddles = max(1.0 - NL_GROUND_RAIN_PUDDLES * fastRand(tiledCpos.xz), 0.0);

      #ifndef NL_GROUND_REFL
      wetness *= puddles;
      float reflective = wetness * rainFactor * NL_GROUND_RAIN_WETNESS;
      #else
      float reflective = NL_GROUND_REFL;
      if (!end && !nether) {
        reflective *= wetness;
      }

      wetness *= puddles;
      reflective = mix(reflective, wetness, rainFactor);
      #endif

      if (wPos.y < 0.0) {
        wetRefl.rgb = getSkyRefl(horizonEdgeCol, horizonCol, zenithCol, viewDir, FOG_COLOR, t, -wPos.y, rainFactor, end, underWater, nether);
        wetRefl.a = calculateFresnel(cosR, 0.03) * reflective;

        #if defined(NL_GROUND_AURORA_REFL) && defined(NL_AURORA) && defined(NL_GROUND_REFL)
        vec2 parallax = viewDir.xz / viewDir.y;
        vec2 projectedPos = wPos.xz - parallax * 100.0;
        float fade = clamp(2.0 - 0.004 * length(projectedPos), 0.0, 1.0);

        vec4 aurora = renderAurora(projectedPos.xyy, t, rainFactor, horizonEdgeCol);
        wetRefl.rgb += 2.0 * aurora.rgb * aurora.a * fade;
        #endif

        wetRefl.rgb += torchColor * lit.x * NL_TORCH_INTENSITY;
        wetRefl.a *= clamp(2.0 - 2.0 * camDist / endDist, 0.0, 1.0); // fade out before clip
      }
    }

    color.rgb *= 1.0 - 0.4 * wetness * rainFactor; // darken wet parts

  #ifndef NL_GROUND_REFL
  }
  #endif

  return wetRefl;
}

#endif // RAIN_H
