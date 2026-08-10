import { useState, useEffect, useRef } from 'react'

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

  return (
    <dialog ref={diagRef} onClose={onClose}>
      <form>
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
