
#ifndef _UNDO_H_
#define _UNDO_H_
//------------------------------------------------------------
//  Undo/Redo for MFC By Keith Rule
class undo {
private:
	CObList	m_undolist;		// Stores undo states
	CObList	m_redolist;		// Stores redo states
	long	m_growsize;		// Adjust for faster saves
	long	m_undoLevels;	// Requested Undolevels 
	long	m_chkpt;

	void AddUndo(CMemFile*);
	void AddRedo(CMemFile *pFile);
	void Load(CMemFile*);
	void Store(CMemFile*);
	void ClearRedoList();

public:

	// Here are the hooks into the CDocument class
	virtual void Serialize(CArchive& ar) = 0;
	virtual void DeleteContents() = 0;

	// User accessable functions
	undo(long undolevels = 8, long = 32768);
	// Constructor
	~undo();			// Destructor
	BOOL CanUndo();		// Returns TRUE if can Undo
	BOOL CanRedo();		// Returns TRUE if can Redo
	void Undo();		// Restore next Undo state
	void Redo();		// Restore next Redo state				
	void CheckPoint();	// Save current state 
	void EnableCheckPoint();
	void DisableCheckPoint();
};

// Constructor
inline undo::
undo(long undolevels, long growsize) :
	m_growsize(growsize), m_undoLevels(undolevels),
	m_chkpt(0)
{
	;
}

// Remove contents of the redo list
inline void undo::
ClearRedoList()
{
	// Clear redo list
	POSITION pos = m_redolist.GetHeadPosition();
	CMemFile* nextFile = NULL;
	while (pos) {
		nextFile = (CMemFile *)m_redolist.GetNext(pos);
		delete nextFile;
	}
	m_redolist.RemoveAll();
}

// Destructor
inline undo::
~undo()
{
	// Clear undo list
	POSITION pos = m_undolist.GetHeadPosition();
	CMemFile  *nextFile = NULL;
	while (pos) {
		nextFile = (CMemFile *)m_undolist.GetNext(pos);
		delete nextFile;
	}
	m_undolist.RemoveAll();

	// Clear redo list
	ClearRedoList();
}

// Checks undo availability, may be used to enable menus
inline BOOL undo::
CanUndo()
{
	return (m_undolist.GetCount() > 1);
}

// Checks redo availability, may be used to enable menus
inline BOOL undo::
CanRedo()
{
	return (m_redolist.GetCount() > 0);
}

// Adds state to the beginning of undo list		
inline void undo::
AddUndo(CMemFile* file)
{
	// Remove old state if there are more than max allowed
	if (m_undolist.GetCount() > m_undoLevels) {
		CMemFile* pFile = (CMemFile *)m_undolist.RemoveTail();
		delete pFile;
	}
	// Add new state to head of undo list
	m_undolist.AddHead(file);
}

// Saves current object into CMemFile instance
inline void undo::
Store(CMemFile* file)
{
	file->SeekToBegin();
	CArchive ar(file, CArchive::store);
	Serialize(ar);
	ar.Close();
}

// Loads CMemfile instance to current object
inline void undo::
Load(CMemFile* file)
{
	DeleteContents();
	file->SeekToBegin();
	CArchive ar(file, CArchive::load);
	Serialize(ar);
	ar.Close();
}

// Save current object state to Undo list
inline void undo::
CheckPoint()
{
	if (m_chkpt <= 0) {
		CMemFile* file = new CMemFile(m_growsize);
		Store(file);
		AddUndo(file);
		ClearRedoList();
	}
}

inline void undo::
EnableCheckPoint()
{
	if (m_chkpt > 0) {
		m_chkpt--;
	}
}

inline void undo::
DisableCheckPoint()
{
	m_chkpt++;
}

// Place CMemFile instnace on Redo list
inline void undo::
AddRedo(CMemFile *file)
{
	// Move state to head of redo list
	m_redolist.AddHead(file);
}

// Perform an Undo command
inline void undo::
Undo()
{
	if (CanUndo()) {
		// Remember that the head of the undo list
		// is the current state. So we just move that
		// to the Redo list and load then previous state.
		CMemFile *pFile = (CMemFile *)m_undolist.GetHead();
		m_undolist.RemoveHead();
		AddRedo(pFile);
		pFile = (CMemFile *)m_undolist.GetHead();
		Load(pFile);
	}
}

//Perform a Redo Command
inline void undo::
Redo()
{
	if (CanRedo()) {
		CMemFile *pFile = (CMemFile *)m_redolist.GetHead();
		m_redolist.RemoveHead();
		AddUndo(pFile);
		Load(pFile);
	}
}

#endif
