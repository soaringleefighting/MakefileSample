/*
 * Copyright (c) 2000, 2001, 2002 Fabrice Bellard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVUTIL_CPU_H
#define AVUTIL_CPU_H

#include <stddef.h>
#include <stdint.h>
#include "attributes.h"


enum AVOptionType{
	AV_OPT_TYPE_FLAGS,
	AV_OPT_TYPE_INT,
	AV_OPT_TYPE_INT64,
	AV_OPT_TYPE_DOUBLE,
	AV_OPT_TYPE_FLOAT,
	AV_OPT_TYPE_STRING,
	AV_OPT_TYPE_RATIONAL,
	AV_OPT_TYPE_BINARY,  ///< offset must point to a pointer immediately followed by an int for the length
	AV_OPT_TYPE_DICT,
	AV_OPT_TYPE_UINT64,
	AV_OPT_TYPE_CONST,
	AV_OPT_TYPE_IMAGE_SIZE, ///< offset must point to two consecutive integers
	AV_OPT_TYPE_PIXEL_FMT,
	AV_OPT_TYPE_SAMPLE_FMT,
	AV_OPT_TYPE_VIDEO_RATE, ///< offset must point to AVRational
	AV_OPT_TYPE_DURATION,
	AV_OPT_TYPE_COLOR,
	AV_OPT_TYPE_CHANNEL_LAYOUT,
	AV_OPT_TYPE_BOOL,
};

/**
 * AVOption
 */
typedef struct AVOption {
    const char *name;

    /**
     * short English help text
     * @todo What about other languages?
     */
    const char *help;

    /**
     * The offset relative to the context structure where the option
     * value is stored. It should be 0 for named constants.
     */
    int offset;
    enum AVOptionType type;

    /**
     * the default value for scalar options
     */
    union {
        int64_t i64;
        double dbl;
        const char *str;
        /* TODO those are unused now */
        //AVRational q;
    } default_val;
    double min;                 ///< minimum valid value for the option
    double max;                 ///< maximum valid value for the option

    int flags;
#define AV_OPT_FLAG_ENCODING_PARAM  1   ///< a generic parameter which can be set by the user for muxing or encoding
#define AV_OPT_FLAG_DECODING_PARAM  2   ///< a generic parameter which can be set by the user for demuxing or decoding
#define AV_OPT_FLAG_AUDIO_PARAM     8
#define AV_OPT_FLAG_VIDEO_PARAM     16
#define AV_OPT_FLAG_SUBTITLE_PARAM  32
/**
 * The option is intended for exporting values to the caller.
 */
#define AV_OPT_FLAG_EXPORT          64
/**
 * The option may not be set through the AVOptions API, only read.
 * This flag only makes sense when AV_OPT_FLAG_EXPORT is also set.
 */
#define AV_OPT_FLAG_READONLY        128
#define AV_OPT_FLAG_BSF_PARAM       (1<<8) ///< a generic parameter which can be set by the user for bit stream filtering
#define AV_OPT_FLAG_FILTERING_PARAM (1<<16) ///< a generic parameter which can be set by the user for filtering
#define AV_OPT_FLAG_DEPRECATED      (1<<17) ///< set if option is deprecated, users should refer to AVOption.help text for more information
//FIXME think about enc-audio, ... style flags

    /**
     * The logical unit to which the option belongs. Non-constant
     * options and corresponding named constants share the same
     * unit. May be NULL.
     */
    const char *unit;
} AVOption;


typedef struct AVClass {
	/**
	* The name of the class; usually it is the same name as the
	* context structure type to which the AVClass is associated.
	*/
	const char* class_name;

	/**
	* A pointer to a function which returns the name of a context
	* instance ctx associated with the class.
	*/
	const char* (*item_name)(void* ctx);

	/**
	* a pointer to the first option specified in the class if any or NULL
	*
	* @see av_set_default_options()
	*/
	const struct AVOption *option;

	/**
	* LIBAVUTIL_VERSION with which this structure was created.
	* This is used to allow fields to be added without requiring major
	* version bumps everywhere.
	*/

	int version;

	/**
	* Offset in the structure where log_level_offset is stored.
	* 0 means there is no such variable
	*/
	int log_level_offset_offset;

	/**
	* Offset in the structure where a pointer to the parent context for
	* logging is stored. For example a decoder could pass its AVCodecContext
	* to eval as such a parent context, which an av_log() implementation
	* could then leverage to display the parent context.
	* The offset can be NULL.
	*/
	int parent_log_context_offset;

	/**
	* Return next AVOptions-enabled child or NULL
	*/
	void* (*child_next)(void *obj, void *prev);

	/**
	* Return an AVClass corresponding to the next potential
	* AVOptions-enabled child.
	*
	* The difference between child_next and this is that
	* child_next iterates over _already existing_ objects, while
	* child_class_next iterates over _all possible_ children.
	*/
	const struct AVClass* (*child_class_next)(const struct AVClass *prev);

	/**
	* Callback to return the supported/allowed ranges.
	* available since version (52.12)
	*/
	int(*query_ranges)(struct AVOptionRanges **, void *obj, const char *key, int flags);
} AVClass;



#define AV_CPU_FLAG_FORCE    0x80000000 /* force usage of selected flags (OR) */

    /* lower 16 bits - CPU features */
#define AV_CPU_FLAG_MMX          0x0001 ///< standard MMX
#define AV_CPU_FLAG_MMXEXT       0x0002 ///< SSE integer functions or AMD MMX ext
#define AV_CPU_FLAG_MMX2         0x0002 ///< SSE integer functions or AMD MMX ext
#define AV_CPU_FLAG_3DNOW        0x0004 ///< AMD 3DNOW
#define AV_CPU_FLAG_SSE          0x0008 ///< SSE functions
#define AV_CPU_FLAG_SSE2         0x0010 ///< PIV SSE2 functions
#define AV_CPU_FLAG_SSE2SLOW 0x40000000 ///< SSE2 supported, but usually not faster
                                        ///< than regular MMX/SSE (e.g. Core1)
#define AV_CPU_FLAG_3DNOWEXT     0x0020 ///< AMD 3DNowExt
#define AV_CPU_FLAG_SSE3         0x0040 ///< Prescott SSE3 functions
#define AV_CPU_FLAG_SSE3SLOW 0x20000000 ///< SSE3 supported, but usually not faster
                                        ///< than regular MMX/SSE (e.g. Core1)
#define AV_CPU_FLAG_SSSE3        0x0080 ///< Conroe SSSE3 functions
#define AV_CPU_FLAG_SSSE3SLOW 0x4000000 ///< SSSE3 supported, but usually not faster
#define AV_CPU_FLAG_ATOM     0x10000000 ///< Atom processor, some SSSE3 instructions are slower
#define AV_CPU_FLAG_SSE4         0x0100 ///< Penryn SSE4.1 functions
#define AV_CPU_FLAG_SSE42        0x0200 ///< Nehalem SSE4.2 functions
#define AV_CPU_FLAG_AESNI       0x80000 ///< Advanced Encryption Standard functions
#define AV_CPU_FLAG_AVX          0x4000 ///< AVX functions: requires OS support even if YMM registers aren't used
#define AV_CPU_FLAG_AVXSLOW   0x8000000 ///< AVX supported, but slow when using YMM registers (e.g. Bulldozer)
#define AV_CPU_FLAG_XOP          0x0400 ///< Bulldozer XOP functions
#define AV_CPU_FLAG_FMA4         0x0800 ///< Bulldozer FMA4 functions
#define AV_CPU_FLAG_CMOV         0x1000 ///< supports cmov instruction
#define AV_CPU_FLAG_AVX2         0x8000 ///< AVX2 functions: requires OS support even if YMM registers aren't used
#define AV_CPU_FLAG_FMA3        0x10000 ///< Haswell FMA3 functions
#define AV_CPU_FLAG_BMI1        0x20000 ///< Bit Manipulation Instruction Set 1
#define AV_CPU_FLAG_BMI2        0x40000 ///< Bit Manipulation Instruction Set 2
#define AV_CPU_FLAG_AVX512     0x100000 ///< AVX-512 functions: requires OS support even if YMM/ZMM registers aren't used

#define AV_CPU_FLAG_ALTIVEC      0x0001 ///< standard
#define AV_CPU_FLAG_VSX          0x0002 ///< ISA 2.06
#define AV_CPU_FLAG_POWER8       0x0004 ///< ISA 2.07

#define AV_CPU_FLAG_ARMV5TE      (1 << 0)
#define AV_CPU_FLAG_ARMV6        (1 << 1)
#define AV_CPU_FLAG_ARMV6T2      (1 << 2)
#define AV_CPU_FLAG_VFP          (1 << 3)
#define AV_CPU_FLAG_VFPV3        (1 << 4)
#define AV_CPU_FLAG_NEON         (1 << 5)
#define AV_CPU_FLAG_ARMV8        (1 << 6)
#define AV_CPU_FLAG_VFP_VM       (1 << 7) ///< VFPv2 vector mode, deprecated in ARMv7-A and unavailable in various CPUs implementations
#define AV_CPU_FLAG_SETEND       (1 <<16)

/**
 * Return the flags which specify extensions supported by the CPU.
 * The returned value is affected by av_force_cpu_flags() if that was used
 * before. So av_get_cpu_flags() can easily be used in an application to
 * detect the enabled cpu flags.
 */
int av_get_cpu_flags(void);

/**
 * Disables cpu detection and forces the specified flags.
 * -1 is a special case that disables forcing of specific flags.
 */
void av_force_cpu_flags(int flags);

/**
 * Set a mask on flags returned by av_get_cpu_flags().
 * This function is mainly useful for testing.
 * Please use av_force_cpu_flags() and av_get_cpu_flags() instead which are more flexible
 */
attribute_deprecated void av_set_cpu_flags_mask(int mask);

/**
 * Parse CPU flags from a string.
 *
 * The returned flags contain the specified flags as well as related unspecified flags.
 *
 * This function exists only for compatibility with libav.
 * Please use av_parse_cpu_caps() when possible.
 * @return a combination of AV_CPU_* flags, negative on error.
 */
// attribute_deprecated
// int av_parse_cpu_flags(const char *s);

/**
 * Parse CPU caps from a string and update the given AV_CPU_* flags based on that.
 *
 * @return negative on error.
 */
int av_parse_cpu_caps(unsigned *flags, const char *s);


#endif /* AVUTIL_CPU_H */
