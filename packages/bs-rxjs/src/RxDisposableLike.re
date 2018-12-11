module type S = {
  type t;

  let dispose: t => unit;

  let isDisposed: t => bool;
};

module type S1 = {
  type t('a);

  let dispose: t('a) => unit;

  let isDisposed: t('a) => bool;
};