open BeltExtension;
open Json;
open Types;

type t =
  | Union(list(t))
  | Number(jsonSchemaNumberItem)
  | Integer(jsonSchemaIntegerItem)
  | String(jsonSchemaStringItem)
  | Boolean
  | Array(jsonSchemaArrayItem)
  | Object(jsonSchemaObjectItem)
  | Null
  | Unknown
and jsonSchemaNumberItem = {enum: list(float)}
and jsonSchemaIntegerItem = {enum: list(int)}
and jsonSchemaStringItem = {enum: list(string)}
and jsonSchemaArrayItem = {item: t}
and jsonSchemaObjectItem = {
  properties: Js.Dict.t(t),
  required: list(string),
};

let rec decoder: Decode.decoder(t) =
  json => {
    open! Decode;

    let union =
      try (
        Some(
          json
          |> oneOf([
               field("anyOf", list(decoder)),
               field("oneOf", list(decoder)),
             ]),
        )
      ) {
      | _ => None
      };

    switch (union) {
    | Some(union) => Union(union)
    | None =>
      let type_ =
        try (Some(json |> field("type", string))) {
        | _ => None
        };

      switch (type_) {
      | Some("number") =>
        Number({
          enum:
            try (json |> field("enum", list(float))) {
            | _ => []
            },
        })
      | Some("integer") =>
        Integer({
          enum:
            try (json |> field("enum", list(int))) {
            | _ => []
            },
        })
      | Some("string") =>
        String({
          enum:
            try (json |> field("enum", list(string))) {
            | _ => []
            },
        })
      | Some("boolean") => Boolean
      | Some("array") => Array({item: json |> field("items", decoder)})
      | Some("object") =>
        Object({
          properties: json |> field("properties", dict(decoder)),
          required:
            try (json |> field("required", list(string))) {
            | _ => []
            },
        })
      | Some("null") => Null
      | _ => Unknown
      };
    };
  };

let rec encoder: encoder(t) =
  fun
  | Number({enum}) =>
    if (List.size(enum) == 0) {
      "decode.num";
    } else {
      let items = enum->List.reduce("", (acc, item) => {j|$item, $acc|j});
      {j|decode.union<number, [$items]>(decode.num, $items)|j};
    }
  | Integer({enum}) =>
    if (List.size(enum) == 0) {
      "decode.int";
    } else {
      let items = enum->List.reduce("", (acc, item) => {j|$item, $acc|j});
      {j|decode.union<number, [$items]>(decode.int, $items)|j};
    }
  | String({enum}) =>
    if (List.size(enum) == 0) {
      "decode.str";
    } else {
      let items = enum->List.reduce("", (acc, item) => {j|"$item", $acc|j});
      {j|decode.union<string, [$items]>(decode.str, $items)|j};
    }
  | Boolean => "decode.bool"
  | Array({item}) => {
      let encoded = encoder(item);
      {j|decode.array($encoded)|j};
    }
  | Object({properties, required}) => {
      let entries = Js.Dict.entries(properties);
      let encoded =
        entries
        |> Js.Array.reduce(
             (acc, (key, item)) => {
               let encodedItem = encoder(item);
               if (List.some(required, (===)(key))) {
                 {j|$key: $encodedItem, $acc|j};
               } else {
                 {j|$key: decode.maybe($encodedItem), $acc|j};
               };
             },
             "",
           );
      {j|decode.object({$encoded})|j};
    }
  | Union(items) => {
      let encoded =
        items->List.map(item => encoder(item))->List.toArray
        |> Js.Array.joinWith(",");
      {j|decode.oneOf($encoded)|j};
    }
  | Null => "decode.nil"
  | Unknown => "decode.nil";