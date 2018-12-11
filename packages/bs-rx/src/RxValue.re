type t('a) = {. "closed": bool};

external asObservable: t('a) => RxObservable.t('a) = "%identity";

[@bs.new] [@bs.module "rxjs"]
external create: 'a => t('a) = "BehaviorSubject";

[@bs.send] external dispose: t('a) => unit = "unsubscribe";

[@bs.send] external getValue: t('a) => 'a = "getValue";

let isDisposed = value => value##closed;

[@bs.send.pipe: t('a)] external next: 'a => unit = "next";

let update = (f, rxValue) => {
  let currentValue = rxValue |> getValue;
  let nextValue = f(currentValue);
  rxValue |> next(nextValue);
};