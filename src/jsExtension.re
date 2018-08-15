open Types;

/* Not doable yet */

/* module type JS = {include (module type of Js);}; */

/* include (Js: JS with module Promise := Js.Promise); */

module type PROMISE = {
  type t('a) = Js.Promise.t('a);

  include PartialFunctor with type t('a) := t('a);
  include PartialMonad with type t('a) := t('a);
};

module Promise: PROMISE = {
  include Js.Promise;

  let map = (f, p) => Js.Promise.then_(x => Js.Promise.resolve(f(x)), p);

  let (>>=) = (p, f) => Js.Promise.then_(f, p);
};