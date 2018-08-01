open Belt
open CommandLineArgs
open Json
open Node
open Prettier
open Util

exception ArgumentError of string

let error =
  try
    match List.fromArray Node.Process.argv with
    | _::_::_::filepath::_ ->
      begin
        match parse @@ Fs.readFileAsUtf8Sync filepath
              |> optionToResult @@ Decode.DecodeError "JSON schema is invalid"
                                   |. Result.map JsonSchema.decoder
                                   |. Result.map JsonSchema.encoder with
        | Result.Ok value    ->
          let promise = resolvePrettierConfig "./prettierrc" in

          let () = ignore (promise |> Js.Promise.then_ (fun options ->
              let options =
                Js.Nullable.toOption options
                |. Option.mapWithDefault
                  defaultPrettierOptions
                  (fun options -> let () = parserSet options "typescript" in options) in
              Js.Promise.resolve @@ Js.log @@ prettify value options
            )) in
          None
        | Result.Error error -> raise error
      end
    | _                 -> raise @@ ArgumentError "Please provide a filepath"
  with
  | ArgumentError error      -> Some {j|Argument error: $error|j}
  | Decode.DecodeError error -> Some {j|Decoder error: $error|j}
  | error                    -> Some {j|Unknown error: $error|j}

let () = match error with
  | Some error -> Js.log error
  | _          -> ()

let () = Js.log2 parsedArgs.input parsedArgs.output
