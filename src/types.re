type encoder('a) = 'a => string;

/* Very basic category theory */
module type PartialFunctor = {
  type t('a);

  let map: ('a => 'b, t('a)) => t('b);
};

module type PartialAlt = {
  type t('a);

  let (<|>): (t('a), t('a)) => t('a);
};

module type PartialMonad = {
  type t('a);

  let (>>=): (t('a), 'a => t('b)) => t('b);
};