
local lpeg = require "lpeg"

function lpeg.St(t)
  local patt = lpeg.P(false)
  for _,v in ipairs(t) do
    patt = patt + lpeg.P(v)
  end
  return patt
end

function lpeg.Cst(t)
  local patt = lpeg.P(false)
  for i,v in ipairs(t) do
    patt = patt + lpeg.P(v) * lpeg.Cc(i)
  end
  return patt
end

-- P, R, S, St = lpeg.P, lpeg.R, lpeg.S, lpeg.St
local P = lpeg.P
local R = lpeg.R
local S = lpeg.S
local St = lpeg.St

-- C, Cc, Cg, Cb, Cmt, Cst = lpeg.C, lpeg.Cc, lpeg.Cg, lpeg.Cb, lpeg.Cmt, lpeg.Cst
local C = lpeg.C
local Cc = lpeg.Cc
local Cp = lpeg.Cp
local Cg = lpeg.Cg
local Cb = lpeg.Cb
local Cmt = lpeg.Cmt
local Cst = lpeg.Cst

local Keywords = {
  "void", "null",
  "bool", "true", "false",
  "char", "byte", "int8",
  "half", "uhalf",
  "full", "ufull",
  "long", "ulong",
  "cent", "ucent",
  "iptr", "uptr",
  "int", "uint",
  "float", "double", "real",
  "var", "imm", "enum", "class",
  "continue", "fallthrough", "break", "goto", "return"
}

local Newlines = {
  "\x0A\x0D",
  "\x0D\x0A",
  "\x0D",
  "\x0A",
  "\x00",
  "\x1A",
  "\xFE\xBF",
  --toUtf8(0x2028),
  --toUtf8(0x2029)
}

local Lcontext = {
  KEYWORD = {n = 1, s = "KEYWORD", h = "KW"};
  SPACE = {n = 2, s = "SPACE", h = "SP"};
  NEWLINE = {n = 3, s = "NEWLINE", h = "NL"};
  COMMENT = {n = 4, s = "COMMENT", h = "CM"};
  BLOCKCOMMENT = {n = 5, s = "BLOCKCOMMENT", h = "BC"};
  STRING = {n = 6, s = "STRING", h = "ST"};
  CHARACTER = {n = 7, s = "CHARACTER", h = "CH"};
  SPECIALIDENTIFIER = {n = 8, s = "SPECIALIDENTIFIER", h = "SI"};
  IDENTIFIER = {n = 9, s = "IDENTIFIER", h = "ID"};
  FLOAT = {n = 10, s = "FLOAT", h = "FL"};
  INTEGER = {n = 11, s = "INTEGER", h = "IN"};
  SPECIALOPERATOR = {n = 12, s = "SPECIALOPERATOR", h = "SO"};
  OPERATOR = {n = 13, s = "OPERATOR", h = "OP"};
  BLACKSLASH = {n = 14, s = "BLACKSLASH", h = "BS"};
  BRACKET = {n = 15, s = "BRACKET", h = "BR"};
  INVALIDTOKEN = {n = 16, s = "INVALIDTOKEN", h = "IV"};
  tokenlineno = 1;
  tokencolumn = 1;
  lineno = 1;
  column = 1;
  blanklist = nil;
  blanktail = nil;
}

local Pstartmatch = P(function () {
  Lcontext.tokenlineno = Lcontext.lineno
  Lcontext.tokencolumn = Lcontext.column
})

local function LfuncNewToken(tokentype, tokenstr, startpos)
  local tbl = {
    type = tokentype;
    tokenstr = tokenstr;
    startpos = startpos;
    lineno = Lcontext.tokenlineno;
    column = Lcontext.tokencolumn;
    occupiedlines = 1;
    tailingblank = nil;
  }
  if tokentype ~= Lcontext.SPACE and tokentype ~= Lcontext.NEWLINE then
    tbl.tailingblank = Lcontext.blanklist
    Lcontext.blanklist = nil
    Lcontext.blanktail = nil
    return tbl
  end
  if Lcontext.blanklist == nil then
    Lcontext.blanklist = tbl
    Lcontext.blanktail = tbl
  else
    Lcontext.blanktail.tailingblank = tbl
    Lcontext.blanktail = tbl
  end
  return tbl
end


-- blank

local Pspace = Cmt(Pstartmatch * Cp() * C(S"\x20\x09\x0B\x0C"^1), function (subject, curpos, startpos, tokenstr)
  LfuncNewToken(Lcontext.SPACE, startpos, tokenstr)
  return curpos
end)

local Pnewline = Cmt(Pstartmatch * Cp() * Cst(Newlines), function (subject, curpos, startpos, i)
  LfuncNewToken(Lcontext.NEWLINE, startpos, Newlines[i])
  Lcontext.lineno = Lcontext.lineno + 1
  Lcontext.column = 1
  return curpos
end)

local Pblankopt = (Pnewline + Pspace)^1 + P""


-- keyword

local Ckeyword = Cst(Keywords) / function (i) return Keywords[i] end

local LKeyword = (Pstartmatch * Cp() * Ckeyword * Pblankopt) / function (startpos, tokenstr, blank)
  return LfuncNewToken(Lcontext.KEYWORD, startpos, tokenstr)
end
