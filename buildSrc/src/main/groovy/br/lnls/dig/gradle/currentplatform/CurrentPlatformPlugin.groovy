package br.lnls.dig.gradle.currentplatform

import org.gradle.model.Finalize
import org.gradle.model.Each
import org.gradle.model.Mutate
import org.gradle.model.RuleSource
import org.gradle.nativeplatform.platform.internal.DefaultNativePlatform
import org.gradle.nativeplatform.TargetedNativeComponent
import org.gradle.platform.base.PlatformContainer

public class CurrentPlatformPlugin extends RuleSource {
    @Finalize
    public void addCurrentPlatformIfNoPlatformsWereDefined(
            PlatformContainer platforms) {
        if (platforms.isEmpty())
            platforms.add(new DefaultNativePlatform("current"))
    }

    @Mutate
    public void setTargetPlatformOfNativeComponentsIfNecessary(
            @Each TargetedNativeComponent component) {
        if (component.targetPlatforms.isEmpty())
            component.targetPlatform 'current'
    }
}
