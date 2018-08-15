open Types;

module type BELT = {include (module type of Belt);};

include (Belt: BELT with module Option := Belt.Option);

module type OPTION = {
  type t('a) = option('a);

  include (module type of Belt.Option);
  include PartialAlt with type t('a) := t('a);
  include PartialFunctor with type t('a) := t('a);
  include PartialMonad with type t('a) := t('a);
};

module Option: OPTION = {
  include Belt.Option;

  type t('a) = option('a);

  let (<|>) = (o1, o2) =>
    switch (o1, o2) {
    | (Some(_) as x, _) => x
    | (_, y) => y
    };

  let map = f =>
    fun
    | None => None
    | Some(x) => Some(f(x));

  let (>>=) = (o, f) =>
    switch (o) {
    | None => None
    | Some(x) => f(x)
    };
};