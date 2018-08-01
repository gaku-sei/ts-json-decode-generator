type trailingComma =
  [ `none | `es5 | `all ] [@@bs.deriving { jsConverter = newType }]

type arrowParens =
  [ `avoid | `always ] [@@bs.deriving { jsConverter = newType }]

type proseWrap =
  [ `always | `never | `preserve ] [@@bs.deriving { jsConverter = newType }]

type prettierOptions =
  { mutable printWidth         : int               [@bs.optional];
    mutable tabWidth           : int               [@bs.optional];
    mutable tabs               : bool              [@bs.optional];
    mutable semicolons         : bool              [@bs.optional];
    mutable singleQuote        : bool              [@bs.optional];
    mutable trailingComma      : abs_trailingComma [@bs.optional];
    mutable bracketSpacing     : bool              [@bs.optional];
    mutable jsxBracketSameLine : bool              [@bs.optional];
    mutable arrowParens        : abs_arrowParens   [@bs.optional];
    mutable rangeStart         : float             [@bs.optional];
    mutable rangeEnd           : float             [@bs.optional];
    mutable parser             : string            [@bs.optional];
    mutable filepath           : string            [@bs.optional];
    mutable requirePragma      : bool              [@bs.optional];
    mutable insertPragma       : bool              [@bs.optional];
    mutable proseWrap          : abs_proseWrap     [@bs.optional];
  } [@@bs.deriving abstract]

let defaultPrettierOptions =
  prettierOptions ~parser:"typescript" ()

external resolvePrettierConfig : string -> prettierOptions Js.Nullable.t Js.Promise.t =
  "resolveConfig" [@@bs.module "prettier"]

external prettify : string -> prettierOptions -> string =
  "format" [@@bs.module "prettier"]
