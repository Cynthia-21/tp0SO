package domain;

import domain.criteriosPertenencia.CriterioDePertenencia;
import domain.exceptions.HechoEliminadoException;
import domain.fuentes.Fuente;

import java.util.ArrayList;
import java.util.List;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class Coleccion {
  private String titulo;
  private String descripcion;
  private CriterioDePertenencia criterioDePertenencia;
  private List<Fuente> fuentes;
  private List<Hecho> hechos;

  public Coleccion(String titulo) {
    this.titulo = titulo;
    this.descripcion = "";
    this.criterioDePertenencia = null;
    this.fuentes = new ArrayList<>();
    this.hechos = new ArrayList<>();
  }

  public void agregarHechosDeFuente() {
    this.fuentes.forEach(fuente -> fuente.importarHechos()
        .forEach(hecho -> this.agregarHecho(hecho)));
  }

  //Agrega el hecho a la coleccion SOLO SI CUMPLE EL CRITERIO y no esta eliminado
  public void agregarHecho(Hecho hecho) {
    if (hecho.getEstaEliminado()) {
      throw new HechoEliminadoException();
    }
    if (this.criterioDePertenencia == null || this.criterioDePertenencia.cumpleCriterios(hecho)) {
        this.hechos.add(hecho);
    }
  }

  public void agregarFuente(Fuente fuente) {
    this.fuentes.add(fuente);
  }

  public void revalidarPorCriterio() {
    this.hechos.removeIf(hecho -> !criterioDePertenencia.cumpleCriterios(hecho));
  }

  //Para un print
  @Override
  public String toString() {
    StringBuilder sb = new StringBuilder();
    sb.append("Colección: ").append(titulo).append("\n");
    sb.append("Descripción: ").append(descripcion).append("\n");
    sb.append("Fuentes: ").append(fuentes).append("\n");
    sb.append("Cantidad de hechos: ").append(hechos.size()).append("\n");

//    junto a la info de la coleccion imprime la lista completa de hechos
//    sb.append("Hechos:\n");
//    for (Hecho hecho : listaDeHechos) {
//      sb.append("- ").append(hecho).append("\n");
//    }

    return sb.toString();
  }
}