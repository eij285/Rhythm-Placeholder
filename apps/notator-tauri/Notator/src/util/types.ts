// temp for C++ backend
export type NoteData = {
  pitch: string
  duration: number
}

export type NoteLength = {
  base: 'breve' | 'semibreve' | 'minim' | 'crotchet' | 'quaver' | 'semiquaver' | 'demisemiquaver' | 'hemidemisemiquaver'
  dots: 0 | 1 | 2
  duration: number
}

export type ScoreMeta = {
    // id?
    title: string
    author: string
}

// type ElementBase = {
//   length: NoteLength
//   voice: number
// }

// export type Rest = ElementBase & {
//   kind: 'rest'
// }

// export type Note = ElementBase & {
//   kind: 'note'
//   pitch: string
// }

// export type Chord = ElementBase & {
//   kind: 'chord'
//   pitches: string[]
// }

// export type MusicalElement = Rest | Note | Chord