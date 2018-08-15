open BeltExtension;
open Json;
open Types;
open Util;

type t = {
  import: string,
  item: JsonSchemaItem.t,
  name: string,
  schema: string,
};

/* TODO: Get a new argument that is the title overload */
let decoder =
    (importName: option(string), importPath: option(string))
    : Decode.decoder(t) =>
  json => {
    open Decode;
    open JsonSchemaItem;
    let title =
      try (
        switch (field("title", string, json)) {
        | "" => None
        | title => Some(title)
        }
      ) {
      | _ => None
      };
    let importName =
      Option.(
        importName
        |> Option.map(
             fun
             | "default" => "Default"
             | importName => importName,
           )
        <|> title
        |> flip(Option.getWithDefault, "Default")
      );
    let importInstruction =
      importName === "Default" ? importName : {j|{ $importName }|j};
    let importPath =
      Option.(
        importPath
        <|> (
          title |> Option.map(Util.uncapitalize) |> Option.map((++)("./"))
        )
        |> flip(Option.getWithDefault, "./index")
      );
    {
      import: {j|import $importInstruction from '$(importPath)'|j},
      item: decoder(json),
      name: importName,
      schema: field("$schema", string, json),
    };
  };

let encoder: encoder(t) =
  ({import, item, name}: t) => {
    let encoded = JsonSchemaItem.encoder(item);
    {j|
import * as decode from 'ts-json-decode';

$import;

const decoder: decode.Decoder<$name> = $encoded;

export default decoder;
    |j};
  };