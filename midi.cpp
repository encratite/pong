#include "midi.hpp"

midi_note::midi_note()
{
}

midi_note::midi_note(unsigned note, unsigned frames):
	note(note),
	remaining_frames(frames)
{
}

bool midi_note::tick()
{
	remaining_frames--;
	return remaining_frames == 0;
}

midi_player::midi_player()
{
	midiOutOpen(&midi_handle, 0, 0, 0, CALLBACK_NULL);
	midiOutShortMsg(midi_handle, 0x50c0);
}

void midi_player::play_note(unsigned note, unsigned frames)
{
	notes.push_back(midi_note(note, frames));
	midiOutShortMsg(midi_handle, (note << 8) | 0x400090);
}

void midi_player::process_frame()
{
	for(std::size_t i = 0; i < notes.size(); i++)
	{
		midi_note & current_note = notes[i];
		bool note_is_done = current_note.tick();
		if(note_is_done)
		{
			midiOutShortMsg(midi_handle, (current_note.note << 8) | 0x80);
			notes.erase(notes.begin() + i);
		}
	}
}
