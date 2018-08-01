type argsRecord =
  { input  : string option;
    output : string option;
  } [@@bs.deriving { jsConverter = newType }]

val args : argsRecord
