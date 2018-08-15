open BeltExtension;
open CommandLineArgs;
open Json;
open JsExtension;
open NodeExtension;
open Prettier;
open Util;

exception ArgumentError(string);

try (
  switch (args) {
  | {input: None} => raise(ArgumentError("Please provide a filepath"))
  | {input: Some(input), output, importName, importPath} =>
    switch (
      parseOrRaise(Fs.readFileAsUtf8Sync(input))
      |> JsonSchema.decoder(importName, importPath)
      |> JsonSchema.encoder
    ) {
    | value =>
      resolvePrettierConfig("./prettierrc")
      |> Promise.map(options => {
           let prettyValue =
             prettify(
               value,
               Option.mapWithDefault(
                 Js.Nullable.toOption(options),
                 defaultPrettierOptions,
                 options => {
                   parserSet(options, "typescript");
                   options;
                 },
               ),
             );
           switch (output) {
           | None => Js.log(prettyValue)
           | Some(output) =>
             let paths =
               incrementalSplit(output, ~separator=Path.separator, ());
             Belt.List.forEachWithIndex(paths, (index, path) =>
               if (index + 1 === List.length(paths)) {
                 Fs.writeFileAsUtf8Sync(output, prettyValue);
               } else {
                 try (Fs.mkdir(path)) {
                 | _ => ()
                 };
               }
             );
           };
         })
      |> ignore
    | exception error => raise(error)
    }
  }
) {
| ArgumentError(error) => Js_console.error({j|Argument error: $error|j})
| Decode.DecodeError(error) => Js_console.error({j|Decoder error: $error|j})
| error => Js_console.error({j|Unknown error: $error|j})
};