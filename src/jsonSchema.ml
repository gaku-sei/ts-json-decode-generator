open Json

type t =
  { item   : JsonSchemaItem.t;
    schema : string;
    title  : string option;
  }

let decoder json =
  let open Decode in
  let open JsonSchemaItem in
  { item   = decoder json;
    schema = json |> field "$schema" string;
    title  = try Some (json |> field "title" string) with _ -> None;
  }

let encoder { item; title } =
  let encoded = JsonSchemaItem.encoder item in
  {j|
  import * as decode from 'ts-json-decode';

  import $title from './$(title)';

  const decoder: decode.Decoder<$title> = $encoded;

  export default decoder;
  |j}
