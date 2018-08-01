type primitiveClass

type commandLineArgsOptions =
  { mutable name           : string;
    mutable alias          : string         [@bs.optional];
    mutable type_          : primitiveClass [@bs.as "type"];
    mutable multiple       : bool           [@bs.optional];
    mutable defaultOptions : bool           [@bs.optional];
  } [@@bs.deriving abstract]

type argsRecord =
  { input  : string option;
    output : string option;
  } [@@bs.deriving { jsConverter = newType }]

let getPrimitiveClass = function
  | `String  -> [%raw "String"]
  | `Number  -> [%raw "Number"]
  | `Boolean -> [%raw "Boolean"]

let requiredArgs =
  [|commandLineArgsOptions ~name:"input" ~alias:"i" ~type_:(getPrimitiveClass `String) ();
    commandLineArgsOptions ~name:"output" ~alias:"o" ~type_:(getPrimitiveClass `String) ();
  |]

external commandLineArgs : commandLineArgsOptions array -> abs_argsRecord =
  "command-line-args" [@@bs.module]

let args =
  argsRecordFromJs @@ commandLineArgs requiredArgs
