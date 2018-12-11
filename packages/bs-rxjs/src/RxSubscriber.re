type t('a) = {. "closed": bool};

external asDisposable: t('a) => RxDisposable.t = "%identity";

[@bs.send] external complete: t('a) => unit = "complete";

[@bs.send] external dispose: t('a) => unit = "unsubscribe";

let isDisposed = subscriber => subscriber##closed;

[@bs.send.pipe: t('a)] external next: 'a => unit = "next";

[@bs.send.pipe: t('a)] external error: exn => unit = "error";