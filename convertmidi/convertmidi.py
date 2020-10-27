import xml.etree.ElementTree as ET
import sys


root = ET.parse(sys.argv[1]).getroot()


def getFrequency(step, octave, alter):

    a4 = 440  # referencal note a4
    a = 2**(1/12)

    octaveSemitones = 12

    notes = {'C': 0,  'D': 2, 'E': 4,  'F': 5,  'G': 7, 'A': 9, 'B': 11}

    diff = (notes[step] + int(octave) * octaveSemitones) - \
        (notes['A'] + 4 * octaveSemitones)

    if alter is not None:
        diff += int(alter)

    return a4 * a ** diff


for part in root.findall('part'):
    print(part.attrib['id'])
    bpm = 120
    sound = part.find('.//sound')

    if sound is not None:
        bpm = int(sound.attrib['tempo'])

    print(bpm, "bpm")

    beatTime = 1 / (bpm/60/1000)
    print(beatTime, "beatTime")

    durationTime = beatTime / int(part.find('.//divisions').text)
    print(durationTime, "durationTime")

    def getDuration(note):
        return int(note.find('duration').text) * durationTime

    tieTime = 0

    for measure in part.findall('measure'):
        for note in measure.findall('note'):

            noteDuration = int(getDuration(note))

            tie = note.findall('tie')

            if len(tie) > 0:
                if len(tie) == 1:
                    if tie[0].attrib['type'] == 'start':
                        tieTime = noteDuration
                        continue
                    if tie[0].attrib['type'] == 'stop':
                        noteDuration = noteDuration + tieTime
                else:
                    tieTime = tieTime + noteDuration
                    continue

            pitch = note.find('pitch')
            if note.find('rest') is not None:
                print('{0, ' + str(noteDuration)+'},', end='')
            else:

                step = getattr(pitch.find('step'), "text", None)
                octave = getattr(pitch.find('octave'), "text", None)
                alter = getattr(pitch.find('alter'), "text", None)

                print('{' + str(round(getFrequency(step, octave, alter))
                                ) + ', ' + str(noteDuration)+'},', end='')
