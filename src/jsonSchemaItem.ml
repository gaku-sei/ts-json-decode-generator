open Belt
open Json

type t =
  | Number
  | Integer
  | String of jsonSchemaStringItem
  | Boolean
  | Array of jsonSchemaArrayItem
  | Object of jsonSchemaObjectItem
  | Null
  | Unknown

and jsonSchemaStringItem =
  { enum : string list;
  }

and jsonSchemaArrayItem =
  { item : t;
  }

and jsonSchemaObjectItem =
  { properties : t Js.Dict.t;
    required   : string list;
  }

let rec decoder json =
  let open Decode in
  match json |> field "type" string with
  | "number"  ->
    Number

  | "integer" ->
    Integer

  | "string"  ->
    String
      { enum = try json |> field "enum" (list string) with _ -> [];
      }

  | "boolean" ->
    Boolean

  | "array"   ->
    Array
      { item = json |> field "items" decoder
      }

  | "object"  ->
    Object
      { properties = json |> field "properties" (dict decoder);
        required   = try json |> field "required" (list string) with _ -> [];
      }

  | "null"    ->
    Null

  | _         -> Unknown

let rec encoder = function
  | Number                          ->
    "decode.num"

  | Integer                         ->
    "decode.num"

  | String { enum; }                ->
    if List.size enum == 0 then
      "decode.str"
    else
      let items = List.reduce enum "" (fun acc item -> {j|"$item", $acc|j}) in
      {j|decode.union<string, [$items]>(decode.str, $items)|j}

  | Boolean                         ->
    "decode.bool"

  | Array { item }                  ->
    let encoded = encoder item in
    {j|decode.array($encoded)|j}

  | Object { properties; required } ->
    let entries = Js.Dict.entries properties in
    let encoded =
      Js.Array.reduce begin
        fun acc (key, item) ->
          let encodedItem = encoder item in
          if List.some required ((==) key) then
            {j|$key: $encodedItem, $acc|j}
          else
            {j|$key: decode.maybe($encodedItem), $acc|j}
      end "" entries in
    {j|decode.object({$encoded})|j}

  | Null                            ->
    "decode.nil"

  | Unknown                         ->
    "decode.nil"
