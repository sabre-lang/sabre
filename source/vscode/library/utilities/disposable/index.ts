/// Sabre Modules
import { Inversify } from '../vendors';

/** Disposable Interface. */
@Inversify.injectable()
export abstract class Disposable {
    abstract dispose(): void;
}

export namespace Disposable {
    //  TYPEDEFS  //

    /** Disposable Callback Typing. */
    export type Callback = () => any;

    /** Any Disposable Typing. */
    export type Any = Callback | Disposable;

    /** Potentially a disposable. */
    export interface Maybe extends Partial<Disposable> {}

    //  IMPLEMENTATIONS  //

    /** Allows internal disposal registration. */
    @Inversify.injectable()
    export class Registry extends Disposable {
        //  PROPERTIES  //

        /** Underlying Stack Instance. */
        private m_subscribers: Callback[] = [];

        //  CONSTRUCTORS  //

        /** Handles disposing of the internal stack. */
        dispose() {
            for (const d of this.m_subscribers) d();
            this.m_subscribers = []; // and remove
        }

        //  PRIVATE METHODS  //

        /**
         * Allow diposal functions.
         * @param handler                   Disposal handler.
         */
        protected m_subscribe<F extends Function>(handler: F): Disposable;

        /**
         * Allow disposing of regular disposables.
         * @param disposable                Disposable to add.
         */
        protected m_subscribe<D extends Disposable>(disposable: D): D;

        /// [Implemenation]
        protected m_subscribe(target: any) {
            // construct the base disposable to be used
            const disposable = from(target);

            // cache the original disposable handle
            const callback = disposable.dispose.bind(disposable);
            this.m_subscribers.push(callback); // push it for use

            // update the disposables "dispose" handler
            disposable.dispose = () => {
                const index = this.m_subscribers.indexOf(callback); // clear now
                if (index > -1) (this.m_subscribers.splice(index, 1), callback());
            };

            // return the disposable now
            return disposable;
        }
    }

    //  FACTORY METHODS  //

    /** Constructs a no-operation disposable. */
    export const noop = (): Disposable =>
        new (class Noop extends Disposable {
            dispose() {}
        })();

    /**
     * Handles joining inline disposables together.
     * @param disposables                           Disposables list.
     * @returns
     */
    export const join = (...disposables: Any[]): Disposable => ({
        dispose: () => disposables.forEach((d) => from(d).dispose()),
    });

    /**
     * Wraps a callback with a `using` handling.
     * @param disposable                            Disposable.
     * @param callback                              Callback instance.
     */
    export const using = <T extends () => any>(disposable: Disposable, callback: T): ReturnType<T> => {
        const result = callback();
        if ('finally' in result) result.finally(() => disposable.dispose());
        else disposable.dispose(); // should remove normally now
        return result; // and should be safe to return
    };

    /**
     * Constructs a disposable from a handler.
     * @param handler               Disposal callback.
     */
    export function from<F extends Callback>(handler: F): Disposable;

    /**
     * Constructs a disposable (no-op).
     * @param disposable            Disposable instance.
     */
    export function from<D extends Disposable>(disposable: D): D;

    /**
     * Constructs a disposable.
     * @param disposable            Disposable instance.
     */
    export function from(disposable: Any): Disposable;

    /// [Implementation]
    export function from(target: Any): Disposable {
        if ('dispose' in target) return target as Disposable;
        else if (typeof target !== 'function') return noop();

        // otherwise construct an in-place instance
        return new (class Anonymous extends Disposable {
            dispose() {
                target();
            }
        })();
    }
}
