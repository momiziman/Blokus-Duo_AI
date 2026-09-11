from alphablokus.config import AlphaZeroConfig


def make_config() -> AlphaZeroConfig:
    """Return the deliberately small configuration used by the first MVP."""
    return AlphaZeroConfig()
