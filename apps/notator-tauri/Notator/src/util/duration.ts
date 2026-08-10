import { NoteLength } from './types'

const noteTypes = new Map<number, NoteLength['base']>([
    [8, 'breve'],
    [4, 'semibreve'],
    [2, 'minim'],
    [1, 'crotchet'],
    [0.5, 'quaver'],
    [0.25, 'semiquaver'],
    [0.125, 'demisemiquaver'],
    [0.0625, 'hemidemisemiquaver']
])

const dotMultipliers = [1, 1.5, 1.75]

export function getNoteType(dur: number): NoteLength {
    for (const [dots, multiplier] of dotMultipliers.entries()) {
        const base = noteTypes.get(dur / multiplier)

        if (base) {
            return { base, dots: dots as 0 | 1 | 2, duration: dur }
        }
    }
    throw new Error(`No note type found for duration ${dur}`)
}
