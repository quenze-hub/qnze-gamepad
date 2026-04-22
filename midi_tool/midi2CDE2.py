import mido

def get_note_name(note_number):
    """MIDIノート番号を音名（C, C#, D...）に変換する"""
    NOTES = ['C', 'CS', 'D', 'DS', 'E', 'F', 'FS', 'G', 'GS', 'A', 'AS', 'B']
    note_name = NOTES[note_number % 12]
    octave = (note_number // 12) - 1
    return f"{note_name}{octave}"

def convert_midi_to_text_with_duration(midi_filepath):
    try:
        mid = mido.MidiFile(midi_filepath)
        print(f"ファイル '{midi_filepath}' を読み込みました。\n")
    except FileNotFoundError:
        print(f"エラー: '{midi_filepath}' が見つかりません。")
        return
    except Exception as e:
        print(f"エラーが発生しました: {e}")
        return

    # MIDIファイルの分解能（4分音符1拍あたりのTick数）
    ticks_per_beat = mid.ticks_per_beat
    # 16分音符あたりのTick数を計算（これを長さ「1」の基準とする）
    ticks_per_16th = ticks_per_beat / 4

    for i, track in enumerate(mid.tracks):
        print(f"--- トラック {i}: {track.name} ---")
        
        events = []        # 計算後の音符・休符データを貯めるリスト
        active_notes = {}  # 現在鳴っている音の「鳴り始めの時間」を管理する辞書
        current_time = 0   # トラック開始からの経過時間（Tick）
        rest_start_time = 0
        
        for msg in track:
            current_time += msg.time
            
            # 発音（Note On）
            if msg.type == 'note_on' and msg.velocity > 0:
                # 全ての音が止まっていた状態からの発音なら、そこまでは休符
                if len(active_notes) == 0:
                    rest_duration = current_time - rest_start_time
                    if rest_duration > 0:
                        length = rest_duration / ticks_per_16th
                        # 0.1未満の極小の休符（手弾きのズレなど）は無視する
                        if length >= 0.1:
                            events.append({
                                'type': 'rest',
                                'start': rest_start_time,
                                'length': length
                            })
                
                # 音が鳴り始めた時間を記録
                active_notes[msg.note] = current_time
                
            # 消音（Note Off、またはvelocity=0のNote On）
            elif msg.type == 'note_off' or (msg.type == 'note_on' and msg.velocity == 0):
                if msg.note in active_notes:
                    # 音の長さを計算
                    start_time = active_notes.pop(msg.note)
                    duration = current_time - start_time
                    length = duration / ticks_per_16th
                    
                    events.append({
                        'type': 'note',
                        'note': msg.note,
                        'start': start_time,
                        'length': length
                    })
                    
                # 全ての音が鳴り止んだら、そこから休符のカウントを開始
                if len(active_notes) == 0:
                    rest_start_time = current_time

        # 集めたイベントを時間が早い順（start）に並び替える
        events.sort(key=lambda x: x['start'])

        # 結果を画面に表示
        k=0;
        for ev in events:
            # {ev['length']:g} を使うことで、4.0は「4」、1.5は「1.5」と綺麗に表示されます
            l=ev['length'];
            length_str = round(l*2)
            #length_str = round(l*100)
            #length_str = l*3*2*2
            k=k+1
            if (k>12):
                k=0;
                print('');
            if ev['type'] == 'note':
                note_str = get_note_name(ev['note'])
                #print('"',end='');
                print(f"N_{note_str},{length_str}",end=',')
            elif ev['type'] == 'rest':
                #print('"',end='');
                if (length_str>0):
                    print(f"N_R,{length_str}",end=',')
                #print(f"N_R,{length_str}",end=',')
        print() # トラックごとの区切り

if __name__ == "__main__":
    # ここに読み込みたいMIDIファイルの名前を指定します
    midi_file = 'sample.mid'  
    convert_midi_to_text_with_duration(midi_file)




    
