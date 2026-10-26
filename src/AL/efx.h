#ifndef SRC_AL_EFX_H
#define SRC_AL_EFX_H

#include <AL/al.h>
#include <AL/alc.h>

#define ALC_EXT_EFX_NAME "ALC_EXT_EFX"
#define AL_EFFECT_TYPE 0x8001
#define AL_EFFECT_REVERB 0x8003
#define AL_REVERB_GAIN 0x0001
#define AL_REVERB_GAINHF 0x0002
#define AL_REVERB_DECAY_TIME 0x0003
#define AL_REVERB_DECAY_HFRATIO 0x0004
#define AL_REVERB_REFLECTIONS_DELAY 0x0005
#define AL_REVERB_REFLECTIONS_GAIN 0x0006
#define AL_REVERB_DIFFUSION 0x0007
#define AL_REVERB_DENSITY 0x0008
#define AL_REVERB_AIR_ABSORPTION_GAINHF 0x0009
#define AL_EFFECTSLOT_EFFECT 0x0001
#define ALC_MAX_AUXILIARY_SENDS 0x20003
#define AL_EFFECT_NULL 0x0000
#define AL_EFFECTSLOT_NULL 0x0000
#define AL_AUXILIARY_SEND_FILTER 0x20002
#define AL_FILTER_NULL 0x0000
#define AL_LOOP_POINTS_SOFT 0x2015

typedef ALvoid (AL_APIENTRY *LPALGENEFFECTS)(ALsizei n, ALuint *effects);
typedef ALvoid (AL_APIENTRY *LPALDELETEEFFECTS)(ALsizei n, const ALuint *effects);
typedef ALvoid (AL_APIENTRY *LPALEFFECTI)(ALuint effect, ALenum param, ALint value);
typedef ALvoid (AL_APIENTRY *LPALEFFECTF)(ALuint effect, ALenum param, ALfloat value);
typedef ALvoid (AL_APIENTRY *LPALGENAUXILIARYEFFECTSLOTS)(ALsizei n, ALuint *effectslots);
typedef ALvoid (AL_APIENTRY *LPALDELETEAUXILIARYEFFECTSLOTS)(ALsizei n, const ALuint *effectslots);
typedef ALvoid (AL_APIENTRY *LPALAUXILIARYEFFECTSLOTI)(ALuint effectslot, ALenum param, ALint value);

#endif // SRC_AL_EFX_H
