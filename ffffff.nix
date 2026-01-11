services.flatpak = {
  enable = true;

  # 1. GLOBALER LOCKDOWN: 
  # Wir verbieten ALLES, was Apps standardmäßig anfordern könnten.
  overrides.global = {
    filesystems = [
      "!home"        # Kein Zugriff aufs Home-Verzeichnis
      "!host"        # Kein Zugriff aufs Host-Filesystem
      "!xdg-config"  # Keine Config-Ordner
    ];
    sockets = [
      "!network"     # Kein Internet
      "!x11"         # Kein unsicheres X11
      "!wayland"     # Selbst Wayland erst mal aus
    ];
    devices = [ "!all" ]; # Keine Webcam/USB/etc.
  };

  # 2. WHITELIST: 
  # Nur was hier steht, wird explizit für die jeweilige App freigeschaltet.
  packages = [
    {
      appId = "org.mozilla.firefox";
      config = {
        # Firefox darf NUR Netzwerk und Wayland nutzen.
        # Erbt "!home" vom globalen Lockdown und bleibt ausgesperrt.
        sockets = [ "network" "wayland" ];
      };
    }
    {
      appId = "com.spotify.Client";
      config = {
        sockets = [ "network" "pulseaudio" ];
      };
    }
  ];
};
