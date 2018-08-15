open BeltExtension;

/* Basics */
let (%) = (f: 'a => 'b, g: 'c => 'a, x: 'c): 'b => f(g(x));

let flip = (f: ('a, 'b) => 'c, x: 'b, y: 'a): 'c => f(y, x);

/* Helpers for option and result */
let optionToResult = (error: exn) =>
  fun
  | Some(value) => Result.Ok(value)
  | None => Result.Error(error);

let resultToOption =
  fun
  | Result.Ok(value) => Some(value)
  | _ => None;

/* Dirty helpers */
let uncapitalize =
  fun
  | "" => ""
  | str =>
    switch (Belt.List.fromArray(Js.String.split("", str))) {
    | [] => ""
    | [head, ...tail] =>
      let tail = Js.Array.joinWith("", List.toArray(tail));
      Js.String.toLowerCase(head) ++ tail;
    };

let rec incrementalSplit =
        (s: string, ~separator: string, ~base: option(string)=?, ())
        : list(string) => {
  let index = Js.String.indexOf(separator, s);
  if (index === (-1)) {
    switch (base) {
    | None => [s]
    | Some(base) => [{j|$base$separator$s|j}]
    };
  } else {
    let head = Js.String.substring(s, ~from=0, ~to_=index);
    let tail =
      Js.String.substring(s, ~from=index + 1, ~to_=Js.String.length(s));
    let completeHead =
      switch (base) {
      | None => head
      | Some(prefix) => {j|$prefix$separator$head|j}
      };
    [
      completeHead,
      ...incrementalSplit(tail, ~separator, ~base=completeHead, ()),
    ];
  };
};