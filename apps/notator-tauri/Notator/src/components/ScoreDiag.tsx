import { useState, useEffect, useRef, type SubmitEvent } from 'react'
import { invoke } from '@tauri-apps/api/core'

type ScoreDiagProps = {
  open: boolean
  onClose: () => void
}

function ScoreDiag({ open, onClose }: ScoreDiagProps) {
  const [title, setTitle] = useState('')
  const [author, setAuthor] = useState('')

  const diagRef = useRef<HTMLDialogElement>(null)
  useEffect(() => {
    if (open) {
      diagRef.current?.showModal()
    } else {
      diagRef.current?.close()
    }
  }, [open])

  function handleSubmit(e: SubmitEvent<HTMLFormElement>) {
    e.preventDefault()
    const result = await invoke('create_score', { title, author })
    console.log(result)
    onClose()
  }

  return (
    <dialog ref={diagRef} onClose={onClose}>
      <form onSubmit={handleSubmit}>
        <label>
          Title
          <input type="text" value={title} onChange={(e) => setTitle(e.target.value)} />
        </label><br/>
        <label>
          Author
          <input type="text" value={author} onChange={(e) => setAuthor(e.target.value)} />
        </label><br/>
        <button type="submit">Create</button>
      </form>
    </dialog>
  );
}

export default ScoreDiag;
