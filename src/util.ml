open Belt

let (%) f g x = f(g(x))

let flip f x y = f y x

let optionToResult error = function
  | Some value -> Result.Ok value
  | None       -> Result.Error error

let resultToOption = function
  | Result.Ok value -> Some value
  | _               -> None
