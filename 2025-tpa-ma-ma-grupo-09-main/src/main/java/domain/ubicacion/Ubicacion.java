package domain.ubicacion;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class Ubicacion {
  private Coordenadas coordenadas;
  private String provincia;
  private String localidad;
  private String calle;
  private Integer altura;

  public Ubicacion(Coordenadas coordenadas) {
    this.coordenadas = coordenadas;
  }

  //para carga manual
  public Ubicacion(String provincia, String localidad, String calle, int altura) {
    this.provincia = provincia;
    this.localidad = localidad;
    this.calle = calle;
    this.altura = altura;
  }

  public boolean esIgualA(Ubicacion otraUbicacion) {
    return this.coordenadas.estaCercaDe(otraUbicacion.getCoordenadas(), 0.05);
  }




//Para un print
  @Override
  public String toString() {
    return "{ " +
        "lat:" + this.coordenadas.getLatitud() +
        ", long:" + this.coordenadas.getLongitud() +
        " }";
  }
}
