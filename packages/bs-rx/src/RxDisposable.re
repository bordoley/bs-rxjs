type t = {. "closed": bool};

type disposable = t;

module type S = {
  type t;

  let asDisposable: t => disposable;
};

module type S1 = {
  type t('a);

  let asDisposable: t('a) => disposable;
};

[@bs.send.pipe: t] external addDisposable: t => t = "add";

[@bs.send.pipe: t]
external addTeardown: ([@bs.uncurry] (unit => unit)) => t = "add";

[@bs.new] [@bs.module "rxjs"] external create: unit => t = "Subscription";

[@bs.new] [@bs.module "rxjs"]
external createWithTeardown: ([@bs.uncurry] (unit => unit)) => t =
  "Subscription";

[@bs.send] external dispose: t => unit = "unsubscribe";

[@bs.val] [@bs.scope "Subscription"] [@bs.module "rxjs"]
external disposed: t = "EMPTY";

let isDisposed = disposable => disposable##closed;

[@bs.send.pipe: t] external removeDisposable: t => t = "remove";