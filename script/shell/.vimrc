" An example for a vimrc file.
"
" Maintainer:	Bram Moolenaar <Bram@vim.org>
" Last change:	2002 Sep 19
"
" To use it, copy it to
"     for Unix and OS/2:  ~/.vimrc
"	      for Amiga:  s:.vimrc
"  for MS-DOS and Win32:  $VIM\_vimrc
"	    for OpenVMS:  sys$login:.vimrc

" When started as "evim", evim.vim will already have done these settings.
if v:progname =~? "evim"
  finish
endif

" Use Vim settings, rather then Vi settings (much better!).
" This must be first, because it changes other options as a side effect.
set nocompatible

" allow backspacing over everything in insert mode
set backspace=indent,eol,start

if has("vms")
  set nobackup		" do not keep a backup file, use versions instead
else
  set backup		" keep a backup file
endif
set history=50		" keep 50 lines of command line history
set ruler		" show the cursor position all the time
set showcmd		" display incomplete commands
set incsearch		" do incremental searching

" For Win32 GUI: remove 't' flag from 'guioptions': no tearoff menu entries
" let &guioptions = substitute(&guioptions, "t", "", "g")

" Don't use Ex mode, use Q for formatting
map Q gq

" This is an alternative that also works in block mode, but the deleted
" text is lost and it only works for putting the current register.
"vnoremap p "_dp

" Switch syntax highlighting on, when the terminal has colors
" Also switch on highlighting the last used search pattern.
if &t_Co > 2 || has("gui_running")
  syntax on
  set hlsearch
endif

" Only do this part when compiled with support for autocommands.
if has("autocmd")

  " Enable file type detection.
  " Use the default filetype settings, so that mail gets 'tw' set to 72,
  " 'cindent' is on in C files, etc.
  " Also load indent files, to automatically do language-dependent indenting.
  filetype plugin indent on

  " Put these in an autocmd group, so that we can delete them easily.
  augroup vimrcEx
  au!

  " For all text files set 'textwidth' to 78 characters.
  autocmd FileType text setlocal textwidth=78

  " When editing a file, always jump to the last known cursor position.
  " Don't do it when the position is invalid or when inside an event handler
  " (happens when dropping a file on gvim).
  autocmd BufReadPost *
    \ if line("'\"") > 0 && line("'\"") <= line("$") |
    \   exe "normal g`\"" |
    \ endif

  augroup END

else

set autoindent		" always set autoindenting on

endif " has("autocmd")
syntax on

if match($TERM, "screen")!=-1
	set term=xterm
	colorscheme  desert
else
	set t_Co=256
	"set background=dark
	colorscheme  wombat256mod 
endif

"source ~/.vim/colors/wombat256mod.vim
"set foldmethod=syntax
filetype plugin on
au BufRead,BufNewFile *.wc set filetype=wcode

"Look in the current directory for "tags", and work up the tree towards root until one is found.
set tags+=~/linuxthread/tags,~/htags,~/tobey.dev/tags,~/inline/tags,~/test.tags
"set tags=~/tmp/tags
source ~/.vim/plugin/gtags.vim
set tabstop=4
set shiftwidth=4
set autoindent
set smartindent
set path=.,~/tobey.dev**
set ru
"taglist
let Tlist_Show_One_File=1
let Tlist_Exit_OnlyWindow=1

set ruler

map <C-^> :bn<CR>
map <S-.> :Gtags<CR>
map <S-r> :Gtags -r<CR>
map <S-o> :Gtags -s<CR>
map <S-f> :Gtags -g<CR> 
map <C-n> :cn<CR>
map <C-p> :cp<CR>
map <C-h> :noh<CR>
map <C-]> :ts <C-R>=expand("<cword>")<CR><CR>
map <S-t> :Tlist<CR>
"set statusline=\ %F%m%r%h%w\ [FORMAT=%{&ff}]\ [TYPE=%Y]\ [ROW=%04l,COL=%04v]\ [%p%%]\ [LEN=%L]\ \ [TIME=%{strftime(\"%m-%d\ %H:%M\")}]
set laststatus=2
