import mido
from mido import MidiFile, MidiTrack, Message

midi_file = MidiFile()
track = MidiTrack()
midi_file.tracks.append(track)

notes = [60, 62, 64, 65, 67, 69, 71, 72]
for note in notes:
    note_on = Message('note_on', note=note, velocity=64, time=0)
    note_off = Message('note_off', note=note, velocity=64, time=960)

    track.append(note_on)
    track.append(note_off)

midi_file.save('hw4.mid')
