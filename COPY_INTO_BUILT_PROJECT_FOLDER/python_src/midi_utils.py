import pandas as pd
import pretty_midi

def convert_data_into_pretty_midi(notes: pd.DataFrame, cancel_event):
    pm = pretty_midi.PrettyMIDI()

    if cancel_event.is_set(): return
    
    # Set the instrument to Acoustic Grand Piano, which is the first in the MIDI standard
    instrument = pretty_midi.Instrument(program=0)  # Program number 0 for Acoustic Grand Piano

    if cancel_event.is_set(): return

    if notes is not None:
        if cancel_event.is_set(): return

        prev_start = 0
        for i, note in notes.iterrows():
            if cancel_event.is_set(): return

            start = float(prev_start + note['step'])
            end = float(start + note['duration'])
            pitch = int(note['pitch'])
            velocity = int(note['velocity'])

            if cancel_event.is_set(): return

            pitch_check = pitch > 127 or pitch < 0
            velocity_check = velocity > 127 or velocity < 0

            if pitch_check or velocity_check:
                continue

            if cancel_event.is_set(): return

            note = pretty_midi.Note(
                velocity=velocity,
                pitch=pitch,
                start=start,
                end=end,
            )

            if cancel_event.is_set(): return

            instrument.notes.append(note)
            prev_start = start

    pm.instruments.append(instrument)
    return pm

def get_midi_data (pm: pretty_midi.PrettyMIDI, df_length, df_limit, cancel_event) -> pd.DataFrame :
    midi_data = []
    max_seq_length = 0

    if cancel_event.is_set(): return

    for instrument in pm.instruments:

        if cancel_event.is_set(): return

        dataframes = []
        if instrument.is_drum == False:

            if cancel_event.is_set(): return

            if len(instrument.notes) != 0:
                if cancel_event.is_set(): return

                temp_notes = instrument.notes
                temp_notes = sorted(instrument.notes, key=lambda note: note.start)

                if cancel_event.is_set(): return

                prev_start = temp_notes[0].start

                note_data_list = []

                if cancel_event.is_set(): return

                for i, note in enumerate(temp_notes):
                    if cancel_event.is_set(): return

                    if df_limit is not None :
                        if len(dataframes) == df_limit :
                            break

                    if cancel_event.is_set(): return

                    note_data = get_note_data(note, prev_start)
                    note_data_list.append(note_data)
                    prev_start = note.start

                    if cancel_event.is_set(): return

                    # If we've reached sequence_size, create a new DataFrame and start a new list
                    current_size = i + 1
                    if current_size % df_length == 0 :
                        if len(note_data_list) > max_seq_length :
                            max_seq_length = len(note_data_list)

                        if cancel_event.is_set(): return

                        dataframes.append(pd.DataFrame(note_data_list))
                        note_data_list = []

                    if cancel_event.is_set(): return
        midi_data.extend(dataframes)

    return midi_data, max_seq_length

def read_all_midi_files (filenames, df_length, df_limit, cancel_event) :
    midi_data = []
    max_seq_length = 0

    if cancel_event.is_set(): return

    for filename in filenames :
        if cancel_event.is_set(): return

        pm = pretty_midi.PrettyMIDI(filename)

        data, cur_max_seq_length = get_midi_data(pm, df_length, df_limit, cancel_event)

        if cancel_event.is_set(): return
        
        if cur_max_seq_length > max_seq_length :
            max_seq_length = cur_max_seq_length
        midi_data.extend(data)

        if cancel_event.is_set(): return

    return midi_data, max_seq_length

def get_note_data (note, prev_start) :
    note_data = {}
    note_data['note_name'] = pretty_midi.note_number_to_name(note.pitch)
    note_data['pitch'] = note.pitch
    note_data['start'] = note.start
    note_data['end'] = note.end
    note_data['prev_start'] = prev_start
    note_data['step'] = note.start - prev_start #gap between the previous note and the next one
    note_data['duration'] = note.end - note.start
    note_data['velocity'] = note.velocity

    return note_data