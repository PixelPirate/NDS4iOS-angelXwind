// Windows/Control/ListView.h

#ifndef __WINDOWS_CONTROL_LISTVIEW_H
#define __WINDOWS_CONTROL_LISTVIEW_H

#include "Windows/Window.h"
#include "Windows/Defs.h"

/*
#include <commctrl.h>
*/

#ifndef _WIN32

#define LVCF_FMT                0x0001
#define LVCF_WIDTH              0x0002
#define LVCF_TEXT               0x0004
#define LVCF_SUBITEM            0x0008
#define LVCF_IMAGE              0x0010
#define LVCF_ORDER              0x0020

#define LVCFMT_LEFT             0x0000
#define LVCFMT_RIGHT            0x0001
#define LVCFMT_CENTER           0x0002
#define LVCFMT_JUSTIFYMASK      0x0003


// state
#define  LVIS_FOCUSED       0x0002 /* wxLIST_STATE_FOCUSED  */
#define  LVIS_SELECTED      0x0004 /* wxLIST_STATE_SELECTED */

#define  LVNI_SELECTED      0x0004 /* wxLIST_STATE_SELECTED */

typedef INT (CALLBACK *PFNLVCOMPARE)(LPARAM, LPARAM, LPARAM);

typedef struct tagLVCOLUMNW
{
    UINT mask;
    int fmt;
    int cx;
    LPWSTR pszText;
    int cchTextMax;
    int iSubItem;
    int iOrder; // FIXME
} LVCOLUMNW;

#define  LVCOLUMN   LVCOLUMNW
#define LV_COLUMNW  LVCOLUMNW  /* FIXME */



typedef struct tagLVITEMW
{
    UINT mask;
    int iItem;
    int iSubItem;
    UINT state;
    UINT stateMask;
    LPWSTR pszText;
    int cchTextMax;
    int iImage;
    LPARAM lParam;
#if (_WIN32_IE >= 0x0300)
    int iIndent;
#endif
#if (_WIN32_WINNT >= 0x501)
    int iGroupId;
    UINT cColumns; // tile view columns
    PUINT puColumns;
#endif
} LVITEMW;

#define LVITEM    LVITEMW

#define LVIF_TEXT   0x0001
// FIXME - mask
#define LVIF_PARAM 2
#define LVIF_IMAGE 4
#define LVIF_STATE 8

#endif

class wxListCtrl;

namespace NWindows {
namespace NControl {

class CListView // : public NWindows::CWindow
{
	wxListCtrl *_list;
public:
	CListView() : _list(0) {}
	void Attach(wxWindow * newWindow);

	operator HWND() const;

	int GetItemCount() const;

	int InsertItem(int index, LPCTSTR text);
	int InsertItem(const LVITEM* item);

	void SetItem(const LVITEM* item);

	int SetSubItem(int index, int subIndex, LPCTSTR text);

	void SetUnicodeFormat(bool fUnicode) { return ;  }

	void InsertColumn(int columnIndex, LPCTSTR text, int width);

	void InsertColumn(int columnIndex, const LVCOLUMNW *columnInfo);

	void DeleteAllItems();

	void SetRedraw(bool);

	void SetItemCount(int );

	void InvalidateRect(void *, bool);

	int GetSelectedCount() const;

	void /* bool */ EnsureVisible(int index, bool partialOK);

	void SetItemState(int index, UINT state, UINT mask);

	UINT GetItemState(int index, UINT mask) const;

	void /* bool */  Update();

	bool DeleteColumn(int columnIndex);

	bool GetItemParam(int itemIndex, LPARAM &param) const;

	int GetNextItem(int startIndex, UINT flags) const;

	int GetFocusedItem() const;

	void RedrawAllItems();
	  // FIXME added
	int GetColumnCount();

	void SetFocus();

	void RedrawItem(int item);

	bool SortItems(PFNLVCOMPARE compareFunction, LPARAM dataParam);

	bool GetColumn(int columnIndex, LVCOLUMN* columnInfo);

	// HWND EditLabel(int itemIndex)
	void EditLabel(int itemIndex);

};

}}
#endif

