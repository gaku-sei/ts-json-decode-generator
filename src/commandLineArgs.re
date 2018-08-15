type primitiveClass;

[@bs.deriving abstract]
type commandLineArgsOptions = {
  mutable name: string,
  [@bs.optional]
  mutable alias: string,
  [@bs.as "type"]
  mutable type_: primitiveClass,
  [@bs.optional]
  mutable multiple: bool,
  [@bs.optional]
  mutable defaultOptions: bool,
};

[@bs.deriving {jsConverter: newType}]
type argsRecord = {
  input: option(string),
  output: option(string),
  importName: option(string),
  importPath: option(string),
};

let getPrimitiveClass =
  fun
  | `String => [%raw "String"]
  | `Number => [%raw "Number"]
  | `Boolean => [%raw "Boolean"];

let requiredArgs = [|
  commandLineArgsOptions(
    ~name="input",
    ~alias="i",
    ~type_=getPrimitiveClass(`String),
    (),
  ),
  commandLineArgsOptions(
    ~name="output",
    ~alias="o",
    ~type_=getPrimitiveClass(`String),
    (),
  ),
  commandLineArgsOptions(
    ~name="importName",
    ~alias="n",
    ~type_=getPrimitiveClass(`String),
    (),
  ),
  commandLineArgsOptions(
    ~name="importPath",
    ~alias="p",
    ~type_=getPrimitiveClass(`String),
    (),
  ),
|];

[@bs.module]
external commandLineArgs: array(commandLineArgsOptions) => abs_argsRecord =
  "command-line-args";

let args: argsRecord = argsRecordFromJs(commandLineArgs(requiredArgs));