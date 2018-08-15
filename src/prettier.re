[@bs.deriving {jsConverter: newType}]
type trailingComma = [ | `none | `es5 | `all];

[@bs.deriving {jsConverter: newType}]
type arrowParens = [ | `avoid | `always];

[@bs.deriving {jsConverter: newType}]
type proseWrap = [ | `always | `never | `preserve];

[@bs.deriving abstract]
type prettierOptions = {
  [@bs.optional]
  mutable printWidth: int,
  [@bs.optional]
  mutable tabWidth: int,
  [@bs.optional]
  mutable tabs: bool,
  [@bs.optional]
  mutable semicolons: bool,
  [@bs.optional]
  mutable singleQuote: bool,
  [@bs.optional]
  mutable trailingComma: abs_trailingComma,
  [@bs.optional]
  mutable bracketSpacing: bool,
  [@bs.optional]
  mutable jsxBracketSameLine: bool,
  [@bs.optional]
  mutable arrowParens: abs_arrowParens,
  [@bs.optional]
  mutable rangeStart: float,
  [@bs.optional]
  mutable rangeEnd: float,
  [@bs.optional]
  mutable parser: string,
  [@bs.optional]
  mutable filepath: string,
  [@bs.optional]
  mutable requirePragma: bool,
  [@bs.optional]
  mutable insertPragma: bool,
  [@bs.optional]
  mutable proseWrap: abs_proseWrap,
};

let defaultPrettierOptions: prettierOptions =
  prettierOptions(~parser="typescript", ());

[@bs.module "prettier"]
external resolvePrettierConfig:
  string => Js.Promise.t(Js.Nullable.t(prettierOptions)) =
  "resolveConfig";

[@bs.module "prettier"]
external prettify: (string, prettierOptions) => string = "format";