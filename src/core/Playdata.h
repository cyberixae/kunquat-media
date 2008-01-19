

#ifndef K_PLAYDATA_H
#define K_PLAYDATA_H


#include <stdint.h>

#include <Pattern.h>
#include <Channel.h>
//#include <Song.h>


/**
 * Playback states.
 */
typedef enum Play_mode
{
	STOP = 0,       ///< Don't play.
	PLAY_PATTERN,   ///< Play one pattern.
	PLAY_SONG,      ///< Play a song.
	PLAY_LAST       ///< Sentinel value -- never used as a mode.
} Play_mode;


#define PAT_CHANNELS (64)


typedef struct Playdata
{
	/**
	 * Current playback mode.
	 */
	Play_mode play;
	/**
	 * Mixing frequency.
	 */
	uint32_t freq;
	/**
	 * Size of a tick in frames.
	 */
	uint16_t tick_size;
	/**
	 * The song to be played. TODO: Song structure
	 */
//	Song* song;
	/**
	 * The number of beats played since the start of playback.
	 */
	Reltime play_time;
	/**
	 * The number of frames mixed since the start of playback.
	 */
	uint64_t play_frames;
	/**
	 * Current tempo.
	 */
	double tempo;
	/**
	 * Current order -- only relevant if \a play = \c PLAY_SONG. TODO: Order structure
	 */
//	uint16_t order;
	/**
	 * Current pattern.
	 */
	uint16_t pattern;
	/**
	 * Current position inside a pattern.
	 */
	Reltime pos;
	/**
	 * The Voice pool used.
	 */
	Voice_pool* voice_pool;
	/**
	 * The channels used.
	 */
	Channel* channels[PAT_CHANNELS];
	/**
	 * The global event queue.
	 */
	Event_queue* events;
} Playdata;


/**
 * Creates a new Playdata object.
 *
 * The caller shall eventually destroy the created object using
 * del_Playdata().
 *
 * \param pool   The Voice pool to be used -- must not be \c NULL.
 * \param song   The Song object to which the new Playdata object is assigned
 *               -- must not be \c NULL.
 *
 * \return   The new Playdata object if successful, or \c NULL if memory
 *           allocation failed.
 */
//Playdata* new_Playdata(Voice_pool* pool, Song* song);


/**
 * Does mixing according to the state of the Playdata object.
 *
 * \param data      The Playdata object -- must not be \c NULL.
 * \param nframes   The number of frames to be mixed.
 *
 * \return   The number of frames actually mixed. This is always
 *           <= \a nframes.
 */
uint32_t Playdata_mix(Playdata* data, uint32_t nframes);


/**
 * Sets the playback mode.
 *
 * \param data   The Playdata object -- must not be \c NULL.
 * \param mode   The playback mode -- must be >= \a STOP and < \a PLAY_LAST.
 */
void Playdata_set_state(Playdata* data, Play_mode mode);


/**
 * Deletes a Playdata object.
 *
 * \param data   The Playdata object -- must not be \c NULL.
 */
void del_Playdata(Playdata* data);


#endif // K_PLAYDATA_H


