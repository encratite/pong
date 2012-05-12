#ifndef MIDI_HPP
#define MIDI_HPP

#include <vector>

#include <windows.h>

struct midi_note
{
	unsigned note;
	unsigned remaining_frames;

	midi_note();
	midi_note(unsigned note, unsigned frames);

	bool tick();
};

class midi_player
{
public:
	midi_player();
	void play_note(unsigned note, unsigned frames);
	void process_frame();

private:
	HMIDIOUT midi_handle;
	std::vector<midi_note> notes;
};

#endif
