import "./App.css";
import { useState, useEffect } from 'react'

import { NoteData, ScoreMeta } from './util/types'
import Scorebook from './components/Scorebook'
import DefaultWin from './components/DefaultWin'
import ScoreDiag from './components/ScoreDiag'

const cMajorScale: NoteData[] = [
  { pitch: 'C4', duration: 0.5 },
  { pitch: 'D4', duration: 0.5 },
  { pitch: 'E4', duration: 0.5 },
  { pitch: 'F4', duration: 0.5 },
  { pitch: 'G4', duration: 0.5 },
  { pitch: 'A4', duration: 0.5 },
  { pitch: 'B4', duration: 0.5 },
  { pitch: 'C5', duration: 0.5 }
]

function App() {
  const [openScores, setOpenScores] = useState<ScoreMeta[]>([])
  const [isScoreDiagOpen, setIsScoreDiagOpen] = useState(false)

  useEffect(() => {
    function handleKeyDown(e: KeyboardEvent) {
      if ((e.metaKey || e.ctrlKey) && e.key === 'n') {
        e.preventDefault()
        setIsScoreDiagOpen(true)
      }
    }

    window.addEventListener('keydown', handleKeyDown)
    return () => window.removeEventListener('keydown', handleKeyDown)
  }, [])

  return (
    <>
      <div>Hello World!</div>
      <main>
        {openScores.length > 0 ? (<Scorebook/>) : (<DefaultWin/>)}
        <ScoreDiag open={isScoreDiagOpen} onClose={() => setIsScoreDiagOpen(false)}/>
      </main>

      {/* <div style={{ display: 'flex', gap: '8px', fontFamily: 'monospace', marginTop: '16px' }}>
        {cMajorScale.map((note, index) => (
          <span key={index}>{note.pitch}q</span>
        ))}
      </div> */}
    </>
  );
}

export default App;
