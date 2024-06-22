# Pack config

# Materials to compile by default
DEFAULT_MATERIALS="RenderChunk Clouds Sky EndSky LegacyCubemap Actor SunMoon"

# Subpacks:
#  OPTIONS   = Subpack options
#  NAMES     = Names/descriptions for options
#  MATERIALS = Materials to compile for options
SUBPACK_OPTIONS=(
  PBR
  DWGR
  FULL
  ULTRA
  MID
  SUB
  PVP
  RREFLECTION
  DEFAULT
)
SUBPACK_NAMES=(
  "pbr"
  "dwgr"
  "full"
  "ultra"
  "mid"
  "Sub"
   "pvp"
   "Rreflection"
  "Default"
)
SUBPACK_MATERIALS=(
   "Clouds ; RenderChunk ;  Sky ; EndSky ; SunMoon"
   "RenderChunk"
  "Clouds ; RenderChunk ;  Sky ; EndSky"
  "Clouds ; RenderChunk"
  "RenderChunk"
  "RenderChunk"
   "RenderChunk"
  "Clouds ; RenderChunk ; Sky ; EndSky"
  ""
)

