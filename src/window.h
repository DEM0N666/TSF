/**
 * This file is part of Tales of Symphonia "Fix".
 *
 * Tales of Symphonia "Fix" is free software : you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by The Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Tales of Symphonia "Fix" is distributed in the hope that it will be
 * useful,
 *
 * But WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Tales of Symphonia "Fix".
 *
 *   If not, see <http://www.gnu.org/licenses/>.
 *
**/
#ifndef __TSFIX__WINDOW_H__
#define __TSFIX__WINDOW_H__

typedef BOOL (WINAPI *MoveWindow_pfn)(
  _In_ HWND hWnd,
  _In_ int  X,
  _In_ int  Y,
  _In_ int  nWidth,
  _In_ int  nHeight,
  _In_ BOOL bRepaint
);

typedef BOOL (WINAPI *SetWindowPos_pfn)(
  _In_ HWND hWnd,
  _In_opt_ HWND hWndInsertAfter,
  _In_ int X,
  _In_ int Y,
  _In_ int cx,
  _In_ int cy,
  _In_ UINT uFlags
);

typedef LONG
(WINAPI *SetWindowLongA_pfn)(
    _In_ HWND hWnd,
    _In_ int nIndex,
    _In_ LONG dwNewLong);

typedef BOOL
(WINAPI *IsIconic_pfn)(
         HWND hWnd);

typedef HWND
(WINAPI *GetForegroundWindow_pfn)(
  void
);

typedef HWND
(WINAPI *GetFocus_pfn)(
  void
);

extern MoveWindow_pfn          MoveWindow_Original;
extern SetWindowPos_pfn        SetWindowPos_Original;
extern GetForegroundWindow_pfn GetForegroundWindow_Original;
extern SetWindowLongA_pfn      SetWindowLongA_Original;
extern IsIconic_pfn            IsIconic_Original;
extern GetFocus_pfn            GetFocus_Original;

#include "command.h"

namespace tsf
{
  // State for window activation faking
  struct window_state_s {
    WNDPROC WndProc_Original = nullptr;

    bool    active           = true;
    bool    activating       = false;

    DWORD   proc_id = 0;
    HWND    hwnd    = nullptr;

    RECT    window_rect;
    RECT    cursor_clip;
    POINT   cursor_pos; // The cursor position when the game isn't screwing
                        //   with it...

    HICON   large_icon;
    HICON   small_icon;

    DWORD   style    = 0x90080000; // Style before we removed the border
    DWORD   style_ex = 0x00000000; // StyleEX before removing the border

    bool    borderless = false;
  } extern window;

  namespace WindowManager
  {
    void Init     ();
    void Shutdown ();

    class CommandProcessor : public eTB_iVariableListener {
    public:
      CommandProcessor (void);

      virtual bool OnVarChange (eTB_Variable* var, void* val = NULL);

      static CommandProcessor* getInstance (void)
      {
        if (pCommProc == NULL)
          pCommProc = new CommandProcessor ();

        return pCommProc;
      }

    protected:
      eTB_Variable* foreground_fps_;
      eTB_Variable* background_fps_;

    private:
      static CommandProcessor* pCommProc;
    };

    class BorderManager {
    public:
      void Enable  (void);
      void Disable (void);
      void Toggle  (void);

      void AdjustWindow (void);
    } static border;
  }
}

#endif /* __TSFIX__WINDOW_H__ */