open Belt

val (%) : ('a -> 'b) -> ('c -> 'a) -> 'c -> 'b

val flip : ('a -> 'b -> 'c) -> 'b -> 'a -> 'c

val optionToResult : exn -> 'a option -> ('a, exn) Result.t

val resultToOption : ('a, _) Result.t -> 'a option
