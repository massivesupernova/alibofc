
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
  "rune", "byte", "int8",
  "ihalf", "uhalf",
  "ifull", "ufull",
  "ilong", "ulong",
  "icent", "ucent",
  "int", "uint", "uptr",
  "float", "double", "freal",
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
  cursor = 1;
  tokenlineno = 1;
  tokencolumn = 1;
  lineno = 1;
  column = 1;
  prevtoken = nil; -- previous nonblank token
  blanktail = nil;
}

local Lheadblank = {
    type = Lcontext.INVALIDTOKEN;
    start = 1;
    text = "";
    lineno = 1;
    column = 1;
    blanks = nil;
    endpos = 1;
}

function Lcontext.reset()
  Lcontext.cursor = 1
  Lcontext.lineno = 1
  Lcontext.column = 1
  Lcontext.prevtoken = nil
  Lcontext.blanktail = nil
end

local Cstart = P(function (subject, curpos)
  Lcontext.tokenlineno = Lcontext.lineno
  Lcontext.tokencolumn = Lcontext.column
  return true, curpos
end)

local function LfuncNewToken(type, start, text, endpos)
  Lcontext.column = Lcontext.column + (endpos - Lcontext.cursor)
  Lcontext.cursor = endpos
  local tk = {
    type = type;
    start = start;
    text = text;
    lineno = Lcontext.tokenlineno;
    column = Lcontext.tokencolumn;
    blanks = nil; -- blanks behind this token
    endpos = endpos;
  }
  print("L"..tk.lineno.." C"..tk.column.." P["..start..","..endpos..")",
      type.s.." #"..#text..":"..text)
  if type ~= Lcontext.SPACE and type ~= Lcontext.NEWLINE then
    Lcontext.prevtoken = tk
    Lcontext.blanktail = tk
    return tk
  end
  if Lcontext.prevtoken == nil then
    Lcontext.prevtoken = Lheadblank
    Lcontext.blanktail = Lheadblank
  end
  Lcontext.blanktail.blanks = tk
  Lcontext.blanktail = tk
  return tk
end


-- blank

local Pspace = Cmt(Cstart * C(S"\x20\x09\x0B\x0C"^1), function (subject, curpos, start, text)
  LfuncNewToken(Lcontext.SPACE, start, text, curpos)
  return curpos
end)

local Pnewline = Cmt(Cstart * Cst(Newlines), function (subject, curpos, start, i)
  LfuncNewToken(Lcontext.NEWLINE, start, Newlines[i], curpos)
  Lcontext.lineno = Lcontext.lineno + 1
  Lcontext.column = 1
  return curpos
end)

--local Pnewline_in_block = ...

local Pblankopt = (Pnewline + Pspace)^1 + P""


-- keyword

local Ckeyword = Cst(Keywords) / function (i) return Keywords[i] end

local Lkeyword = Cmt(Cstart * Ckeyword, function (subject, curpos, start, text)
  return curpos, LfuncNewToken(Lcontext.KEYWORD, start, text, curpos)
end) * Pblankopt


-- identifier

local Pletter = R("az", "AZ")
local Pnumber = R("09")
local Palnum = Pletter + Pnumber

local Cidentifier = C((P"_" + Pletter) * (P"_" + Palnum)^0)

local Lidentifier = (Cstart * Cidentifier * Cp()) / function (start, text, endpos)
  return LfuncNewToken(Lcontext.IDENTIFIER, start, text, endpos)
end


-- bracket

local Cbracket = C(S"(){}[]")

local Lbracket = (Cstart * Cbracket * Cp()) / function (start, text, endpos)
  return LfuncNewToken(Lcontext.BRACKET, start, text, endpos)
end


-- test

local function ltest(expr, value)
  if expr == value then
    return true
  end
  print("ltest failure: " .. tostring(expr) .. " not equal to " .. tostring(value))
  return false
end

do
  local patt = Lkeyword + Lidentifier + Lbracket
  local subject = "[KW]bool \t \n [KW]float  \r  \n\r   \r\n  "
  local hint, token

  Lcontext.reset()

  while true do
    token = patt:match(subject, Lcontext.cursor)
    if token == nil then break end
    assert(ltest(token.text, "["))

    token = patt:match(subject, Lcontext.cursor)
    if token == nil then print"TEST: no hint" break end
    hint = token.text

    token = patt:match(subject, Lcontext.cursor)
    if token == nil then print"TEST: no ]" break end
    assert(ltest(token.text, "]"))

    token = patt:match(subject, Lcontext.cursor)
    if token == nil then print"TEST: no value" break end
    assert(ltest(token.type.h, hint))

    print("\t\t["..token.text.."]")
    local tk = token.blanks
    while tk ~= nil do
      print("\t\t"..tk.lineno.." "..tk.column.." "..tk.start.." "..tk.endpos.." "..tk.type.s.." #"..#tk.text)
      tk = tk.blanks
    end
  end
end
