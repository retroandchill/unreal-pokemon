namespace InteractiveUI.Core;

/// <summary>
/// Represents an interface providing the capability to refresh or update the state of the implementing object.
/// Implementers of this interface define custom behavior for refreshing their internal data or UI components.
/// </summary>
public interface IRefreshable
{
    /// <summary>
    /// Refreshes the state of the object. This method is intended to be implemented by classes
    /// that require their internal state, UI components, or other properties to be updated.
    /// Implementations may define the behavior specific to the context or usage of the object.
    /// </summary>
    void Refresh();
}
