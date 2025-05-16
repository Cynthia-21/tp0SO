package domain.ubicacion;

import lombok.Getter;

@Getter
public class Coordenadas {
  private double latitud;
  private double longitud;

  public Coordenadas(double latitud, double longitud) {
    this.latitud = latitud;
    this.longitud = longitud;
  }

  public boolean estaCercaDe(Coordenadas otrasCoordenadas, double kmALaRedonda) {
    double distancia = this.formulaHavershine(otrasCoordenadas);
    return distancia <= kmALaRedonda;
  }

  private double formulaHavershine(Coordenadas otrasCoordenadas) {
    final int radioTierraKM = 6371;

    double latitud1Radianes = Math.toRadians(this.latitud);
    double latitud2Radianes = Math.toRadians(otrasCoordenadas.getLatitud());
    double deltaLatitud = Math.toRadians(otrasCoordenadas.getLatitud() - this.latitud);
    double deltaLongitud = Math.toRadians(otrasCoordenadas.getLongitud() - this.longitud);

    double a = Math.sin(deltaLatitud / 2) * Math.sin(deltaLatitud / 2)
        + Math.cos(latitud1Radianes) * Math.cos(latitud2Radianes)
        * Math.sin(deltaLongitud / 2) * Math.sin(deltaLongitud / 2);

    double anguloCentral = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
    return radioTierraKM * anguloCentral;
  }
}
