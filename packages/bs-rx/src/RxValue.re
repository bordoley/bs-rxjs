type t('a) = {. "closed": bool};

external asObservable: t('a) => RxObservable.t('a) = "%identity";

[@bs.new] [@bs.module "rxjs"]
external create: 'a => t('a) = "BehaviorSubject";

[@bs.send] external dispose: t('a) => unit = "unsubscribe";

[@bs.send] external getValue: t('a) => 'a = "getValue";

let isDisposed = value => value##closed;

[@bs.send.pipe: t('a)] external next: 'a => unit = "next";

let notify = (prevValue, nextValue, self) =>
  if (prevValue !== nextValue) {
    self |> next(nextValue);
  };

let update = (f, self) => {
  let prevValue = self |> getValue;
  let nextValue = f(prevValue);
  notify(prevValue, nextValue, self);
};

let update1 = (f, ctx0, self) => {
  let prevValue = self |> getValue;
  let nextValue = f(ctx0, prevValue);
  notify(prevValue, nextValue, self);
};

let update2 = (f, ctx0, ctx1, self) => {
  let prevValue = self |> getValue;
  let nextValue = f(ctx0, ctx1, prevValue);
  notify(prevValue, nextValue, self);
};

let update3 = (f, ctx0, ctx1, ctx2, self) => {
  let prevValue = self |> getValue;
  let nextValue = f(ctx0, ctx1, ctx2, prevValue);
  notify(prevValue, nextValue, self);
};

let update4 = (f, ctx0, ctx1, ctx2, ctx3, self) => {
  let prevValue = self |> getValue;
  let nextValue = f(ctx0, ctx1, ctx2, ctx3, prevValue);
  notify(prevValue, nextValue, self);
};

let update5 = (f, ctx0, ctx1, ctx2, ctx3, ctx4, self) => {
  let prevValue = self |> getValue;
  let nextValue = f(ctx0, ctx1, ctx2, ctx3, ctx4, prevValue);
  notify(prevValue, nextValue, self);
};