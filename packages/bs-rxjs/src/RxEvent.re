type t('a) = {. "closed": bool};

external asObservable: t('a) => RxObservable.t('a) = "%identity";

[@bs.new] [@bs.module "rxjs"] external create: unit => t('a) = "Subject";

[@bs.send.pipe: t('a)] external dispatch: 'a => unit = "next";

let isDisposed = event => event##closed;

[@bs.send] external dispose: t('a) => unit = "unsubscribe";