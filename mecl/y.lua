
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

-- P, R, S, St, V = lpeg.P, lpeg.R, lpeg.S, lpeg.St, lpeg.V
local P = lpeg.P
local R = lpeg.R
local S = lpeg.S
local St = lpeg.St
local V = lpeg.V

-- C, Cc, Cg, Cb, Ct, Cmt, Cst = lpeg.C, lpeg.Cc, lpeg.Cg, lpeg.Cb, lpeg.Ct, lpeg.Cmt, lpeg.Cst
local C = lpeg.C
local Cc = lpeg.Cc
local Cp = lpeg.Cp
local Cg = lpeg.Cg
local Cb = lpeg.Cb
local Ct = lpeg.Ct
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
    "T";
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

local function L(type, Ctext)
  return Cmt(Cstart * Ctext, function (subject, curpos, start, text)
    return curpos, LfuncNewToken(type, start, text, curpos)
  end) * Pblankopt
end


-- keyword

local Ckeyword = Cst(Keywords) / function (i) return Keywords[i] end
local Lkeyword = L(Lcontext.KEYWORD, Ckeyword)


-- identifier

local Pletter = R("az", "AZ")
local Pnumber = R("09")
local Palnum = Pletter + Pnumber

local Cidentifier = C((P"_" + Pletter) * (P"_" + Palnum)^0)
local Lidentifier = L(Lcontext.IDENTIFIER, Cidentifier)


-- bracket

local Cbracket = C(S"(){}[]")

local Lbracket = (Cstart * Cbracket * Cp()) / function (start, text, endpos)
  return LfuncNewToken(Lcontext.BRACKET, start, text, endpos)
end


-- ltest

local function Test(expr, value)
  if expr == value then
    return true
  end
  print("Test failure: " .. tostring(expr) .. " not equal to " .. tostring(value))
  return false
end

do
  local patt = Lkeyword + Lidentifier + Lbracket
  local subject = "[KW]bool \t \n [KW]float  \r  \n\r   \r\n  " ..
      "[ID]_ \t\r\n [ID]_ab  [ID]ab1  \n\n [ID]ab_2  "

  local hint, token

  Lcontext.reset()

  while true do
    token = patt:match(subject, Lcontext.cursor)
    if token == nil then break end
    assert(Test(token.text, "["))

    token = patt:match(subject, Lcontext.cursor)
    if token == nil then print"TEST: no hint" break end
    hint = token.text

    token = patt:match(subject, Lcontext.cursor)
    if token == nil then print"TEST: no ]" break end
    assert(Test(token.text, "]"))

    token = patt:match(subject, Lcontext.cursor)
    if token == nil then print"TEST: no value" break end
    assert(Test(token.type.h, hint))

    print("\t\t["..token.text.."]")
    local tk = token.blanks
    while tk ~= nil do
      print("\t\t"..tk.lineno.." "..tk.column.." "..tk.start.." "..tk.endpos.." "..tk.type.s.." #"..#tk.text)
      tk = tk.blanks
    end
  end
end


-- expression

local Lsparn = L(Lcontext.BRACKET, C"(")
local Leparn = L(Lcontext.BRACKET, C")")
local Lsquad = L(Lcontext.BRACKET, C"[")
local Lequad = L(Lcontext.BRACKET, C"]")
local Lscurl = L(Lcontext.BRACKET, C"{")
local Lecurl = L(Lcontext.BRACKET, C"}")

local LEQU = L(Lcontext.OPERATOR, C(S"+-*/%&|^~" * P"=" + P"=" + P"<<=" + P">>=" + P"**="))
local LGOR = L(Lcontext.OPERATOR, C(P"||" + P"or"))
local LGAD = L(Lcontext.OPERATOR, C(P"&&" + P"and"))
local LCMP = L(Lcontext.OPERATOR, C(S"=!><" * P"=" + P"<" + P">"))
local LBOR = L(Lcontext.OPERATOR, C"|")
local LXOR = L(Lcontext.OPERATOR, C"^")
local LBAD = L(Lcontext.OPERATOR, C"&")
local LSHF = L(Lcontext.OPERATOR, C(P"<<" + P">>"))
local LADD = L(Lcontext.OPERATOR, C(S"+-~"))  --> binary ~ for concat, unary ~ for bitwise not
local LMUL = L(Lcontext.OPERATOR, C(S"*/%"))
local LPOW = L(Lcontext.OPERATOR, C"**")       --> binary ** for pow, binary ^ for xor
local LUNA = L(Lcontext.OPERATOR, C(S"+-!~*&#"))
local LDOT = L(Lcontext.OPERATOR, C".")
local LCMA = L(Lcontext.OPERATOR, C",")
local LQST = L(Lcontext.OPERATOR, C"?")
local LCLN = L(Lcontext.OPERATOR, C":")

local VStart = V"CommaExpr"
local VCondExpr = V"CondExpr"
local VBinaryExpr = V"BinaryExpr"
local VLogicalOr = V"LogicalOr"
local VLogicalAnd = V"LogicalAnd"
local VCmpExpr = V"CmpExpr"
local VBitwiseOr = V"BitwiseOr"
local VBitwiseXor = V"BitwiseXor"
local VBitwiseAnd = V"BitwiseAnd"
local VBitwiseShift = V"BitwiseShift"
local VAddExpr = V"AddExpr"
local VMulExpr = V"MulExpr"
local VPowExpr = V"PowExpr"
local VUnaryExpr = V"UnaryExpr"
local VPostfixExpr = V"PostfixExpr"
local VPrimaryExpr = V"PrimaryExpr"

local VExpr = P{
  VStart;
  CommaExpr = Ct(Cc"VCMA" * (VCondExpr * LCMA * VStart + VCondExpr));
  CondExpr = Ct(Cc"VCND" * (VBinaryExpr * LQST * VStart * LCLN * VCondExpr + VBinaryExpr));
  BinaryExpr = Ct(Cc"VBIN" * VLogicalOr * (LGOR * VLogicalOr)^0);
  LogicalOr = Ct(Cc"VBIN" * VLogicalAnd * (LGAD * VLogicalAnd)^0);
  LogicalAnd = Ct(Cc"VBIN" * VCmpExpr * (LCMP * VCmpExpr)^0);
  CmpExpr = Ct(Cc"VBIN" * VBitwiseOr * (LBOR * VBitwiseOr)^0);
  BitwiseOr = Ct(Cc"VBIN" * VBitwiseXor * (LXOR * VBitwiseXor)^0);
  BitwiseXor = Ct(Cc"VBIN" * VBitwiseAnd * (LBAD * VBitwiseAnd)^0);
  BitwiseAnd = Ct(Cc"VBIN" * VBitwiseShift * (LSHF * VBitwiseShift)^0);
  BitwiseShift = Ct(Cc"VBIN" * VAddExpr * (LADD * VAddExpr)^0);
  AddExpr = Ct(Cc"VBIN" * VMulExpr * (LMUL * VMulExpr)^0);
  MulExpr = Ct(Cc"VBIN" * VPowExpr * (LPOW * VPowExpr)^0);
  PowExpr = Ct(Cc"VMID" * (VPostfixExpr + VUnaryExpr));
  UnaryExpr = Ct(Cc"VUNA" * (LUNA * VUnaryExpr + VPostfixExpr));
  PostfixExpr = Ct(Cc"VPST" * VPrimaryExpr * (LDOT * VPrimaryExpr)^0);
  PrimaryExpr = Ct(Cc"VPRM" * (Lidentifier + Lsparn * VStart * Leparn));
}

local function VfuncEvaluate(t)
  assert(Test(type(t), "table"))
  local function write(...) io.stdout:write(...) end
  local v = t[1]
  if v == "VCMA" then
    VfuncEvaluate(t[2])
  elseif v == "VCND" then
    VfuncEvaluate(t[2])
  elseif v == "VBIN" then
    VfuncEvaluate(t[2])
    for i = 3, #t, 2 do
      VfuncEvaluate(t[i+1])
      write(t[i].text)
    end
  elseif v == "VMID" then
    VfuncEvaluate(t[2])
  elseif v == "VUNA" then
    if t[2][1] == "T" then
      write("("..t[2].text)
      VfuncEvaluate(t[3])
      write(")")
    else
      VfuncEvaluate(t[2])
    end
  elseif v == "VPST" then
    VfuncEvaluate(t[2])
  elseif v == "VPRM" then
    if t[2].type == Lcontext.IDENTIFIER then
      write(t[2].text)
    else
      write(t[2].text)
      VfuncEvaluate(t[3])
      write(t[4].text)
    end
  else
    write("Test: invalid variable type")
  end
end


-- vtest

do
  local expr = "a + b * c / d**(e+f) - g * -#h"
  local tbl = nil

  print("\n")
  Lcontext.reset()

  tbl = VExpr:match(expr)
  if tbl == nil then
    print("Test: expression match failed")
  end
  VfuncEvaluate(tbl)
  print"\n"
end
