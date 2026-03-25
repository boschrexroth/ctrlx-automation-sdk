package datalayer.provider.sub;

import java.lang.System.Logger.Level;

import com.boschrexroth.api.NotifyItem;
import com.boschrexroth.api.Result;
import com.boschrexroth.api.VariantType;

final class SubsCallback implements com.boschrexroth.api.SubscriptionCallback {

    private static final java.lang.System.Logger LOG = java.lang.System.getLogger(SubsCallback.class.getName());

    @Override
    public void onSubscription(Result results, NotifyItem[] items) {
        if (results != Result.OK) {
            LOG.log(Level.ERROR, "onSubscription error: {0}", results);
            return;
        }

        for (NotifyItem item : items) {
            if (item.getData().getType() == VariantType.INT32) {
                LOG.log(Level.INFO, "address: {0}, value: {1}", item.getAddress(), item.getData().getInt32());
                continue;
            }
            LOG.log(Level.INFO, "address: {0}, value<?>_type: {1}", item.getAddress(), item.getData().getType());
        }
    }
}