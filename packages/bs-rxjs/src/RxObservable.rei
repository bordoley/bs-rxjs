type t('a);

type observable('a) = t('a);

module type S = {
  type t;
  type a;

  let asObservable: t => observable(a);
};

module type S1 = {
  type t('a);

  let asObservable: t('a) => observable('a);
};

let create: (RxSubscriber.t('a) => unit) => t('a);

let fromEvent: (~capture: bool=?, ~passive: bool=?, string, Dom.eventTarget) => t(Dom.event);

let ofArray: (~scheduler: RxPriorityScheduler.t=?, array('a)) => t('a);

let ofList: (~scheduler: RxPriorityScheduler.t=?, list('a)) => t('a);

let ofValue: (~scheduler: RxPriorityScheduler.t=?, 'a) => t('a);

let subscribe: t('a) => RxDisposable.t;

let toPromise: t('a) => Js.Promise.t('a);